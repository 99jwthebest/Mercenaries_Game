// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Merc_MeleeComponent.h"
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

    AActor* Owner = GetOwner();
    if (!Owner) 
        return;

    FVector Start = Owner->GetActorLocation();
    FVector ForwardVector = Owner->GetActorForwardVector();
    FVector End = Start + ForwardVector * MeleeRange;

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

    UE_LOG(LogTemp, Error, TEXT("Melee Accomplished!!!!"));

    if (bHit)
    {
        for (auto& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor)
            {
                // Apply damage
                UGameplayStatics::ApplyDamage(HitActor, MeleeDamage, Owner->GetInstigatorController(), Owner, nullptr);
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

void UMerc_MeleeComponent::ResetAttackCooldown()
{
	bCanAttack = true;
}