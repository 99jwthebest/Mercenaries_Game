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
				MeleeTraceCalcuation();
            }
        }
    }

}

void UMerc_MeleeComponent::MeleeTrace()
{
    if (!bCanAttack)
        return;

    AActor* Owner = GetOwner();
    if (!Owner)
        return;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + MeleeForwardVector * MeleeRange;

    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(Owner);

    TArray<FHitResult> HitResults;

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

    if (bHit)
    {
        for (auto& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor)
            {
                if (HitActor->GetClass()->ImplementsInterface(UHitDamageable::StaticClass()))
                {
                    UPrimitiveComponent* HitComp = Hit.Component.Get();

                    // Direction: use your cached vector (NOT animation-rotated forward)
                    const FVector Dir = MeleeForwardVector.GetSafeNormal();

                    const FHitSpec Spec = FHitSpec::MakeMelee(
                        Owner,        // Instigator
                        HitComp,      // Hit component for multipliers
                        MeleeDamage,
                        Dir,
                        KnockbackStrength,
                        UpwardBoost,
                        StunTime,
                        true          // Face instigator (RE4 style)
                    );

                    IHitDamageable::Execute_ApplyHit(HitActor, Spec);

                    UE_LOG(LogTemp, Log, TEXT("Requested MELEE hit: BaseDamage=%f to %s"), MeleeDamage, *HitActor->GetName());
                }
            }
        }
    }

    // Start cooldown
    bCanAttack = false;
    GetWorld()->GetTimerManager().SetTimer(
        AttackCooldownTimerHandle,
        this,
        &UMerc_MeleeComponent::ResetAttackCooldown,
        AttackCooldown,
        false
    );
}

FVector UMerc_MeleeComponent::MeleeTraceCalcuation()
{
    AActor* Owner = GetOwner();
    if (!Owner)
        return FVector::ZeroVector;

    MeleeForwardVector = Owner->GetActorForwardVector();

	return MeleeForwardVector;
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
