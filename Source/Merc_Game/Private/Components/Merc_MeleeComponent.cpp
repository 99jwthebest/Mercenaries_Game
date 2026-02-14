// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Merc_MeleeComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Interfaces/IHitDamageable.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMerc_MeleeComponent::UMerc_MeleeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMerc_MeleeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMerc_MeleeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMerc_MeleeComponent::PerformMeleeAttack()
{
    if (!bCanAttack) 
        return;

    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
        {
            if (MeleeAttackMontage) 
            {
				Character->bUseControllerRotationYaw = false;
                //AnimInstance->Montage_Play(MeleeAttackMontage);
				Character->PlayAnimMontage(MeleeAttackMontage);
                // Bind end just for this montage instance
                FOnMontageEnded EndDelegate;
                EndDelegate.BindUObject(this, &UMerc_MeleeComponent::HandleMontageEnded);
                AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);
                CacheMeleeForwardVector();
				StartAttackCooldown();
            }
        }
    }
}

void UMerc_MeleeComponent::OnMeleeAnimNotify()
{
    // optional: allow trace only if currently attacking
    // if (!bIsAttacking) return;

	ApplyMeleeHits(MeleeTrace());
}

FVector UMerc_MeleeComponent::CacheMeleeForwardVector()
{
    AActor* Owner = GetOwner();
    if (!Owner)
        return FVector::ZeroVector;
    // below is optional tweak if we want the trace to start in a more accurate position
    // const FVector Start = Owner->GetActorLocation() + FVector(0, 0, 50.f);

    MeleeForwardVector = Owner->GetActorForwardVector();

	return MeleeForwardVector;
}

TArray<FHitResult> UMerc_MeleeComponent::MeleeTrace() const
{
    TArray<FHitResult> HitResults;
    
    AActor* Owner = GetOwner();
    if (!Owner)
        return HitResults;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + MeleeForwardVector * MeleeRange;

    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(Owner);


    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        Start,
        End,
        50.f, // radius of the melee sweep
        UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
        false,
        IgnoredActors,
        EDrawDebugTrace::ForDuration,
        HitResults,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        5.0f // Duration to display debug lines
    );

    return HitResults;
}

void UMerc_MeleeComponent::ApplyMeleeHits(const TArray<FHitResult>& HitResults)
{
    AActor* Owner = GetOwner();
    if (!Owner) 
        return;

    // Avoid multi-hitting the same enemy in one swing
    TSet<TWeakObjectPtr<AActor>> HitActorsThisSwing;

    const FVector Dir = MeleeForwardVector.GetSafeNormal();

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor) 
            continue;

        if (HitActorsThisSwing.Contains(HitActor)) 
            continue;
        HitActorsThisSwing.Add(HitActor);

        if (!HitActor->GetClass()->ImplementsInterface(UHitDamageable::StaticClass()))
            continue;

        UPrimitiveComponent* HitComp = Hit.Component.Get();

        const FHitSpec Spec = FHitSpec::MakeMelee(
            Owner,
            HitComp,
            MeleeDamage,
            Dir,
            KnockbackStrength,
            UpwardBoost,
            StunTime,
            true
        );

        IHitDamageable::Execute_ApplyHit(HitActor, Spec);

        UE_LOG(LogTemp, Log, TEXT("Requested MELEE hit: BaseDamage=%f to %s"), MeleeDamage, *HitActor->GetName());
    }
}

void UMerc_MeleeComponent::StartAttackCooldown()
{
   bCanAttack = false;
   GetWorld()->GetTimerManager().SetTimer(
       AttackCooldownTimerHandle,
       this,
       &UMerc_MeleeComponent::ResetAttackCooldown,
       AttackCooldown,
       false
   );
}

void UMerc_MeleeComponent::ResetAttackCooldown()
{
	bCanAttack = true;
}

void UMerc_MeleeComponent::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage != MeleeAttackMontage)
        return; // safety

    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        Character->bUseControllerRotationYaw = true;
        if (UAnimInstance* Anim = Character->GetMesh()->GetAnimInstance())
        {
            FOnMontageEnded EndDelegate;
            Anim->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);
        }
	}
}
