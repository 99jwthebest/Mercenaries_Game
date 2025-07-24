// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Merc_BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatTrackerComponent.h"


// Sets default values
AMerc_BaseEnemy::AMerc_BaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMerc_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

// Called every frame
void AMerc_BaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float AMerc_BaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = FMath::Min(CurrentHealth, DamageAmount);
	CurrentHealth -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Zombie took %f damage. Remaining HP: %f"), DamageApplied, CurrentHealth);

	// Save the instigator for later (used in Die)
	LastInstigator = EventInstigator;

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return DamageApplied;
}

void AMerc_BaseEnemy::ResetAttackCooldown()
{
	bCanAttack = true;
	UE_LOG(LogTemp, Warning, TEXT("Zombie cooldown reset. Can attack again."));
}

void AMerc_BaseEnemy::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("Zombie died!"));

	// Reward score to killer
	if (LastInstigator && LastInstigator->GetPawn())
	{
		AActor* Killer = LastInstigator->GetPawn(); // Could be player or bot

		if (UStatTrackerComponent* StatTracker = Killer->FindComponentByClass<UStatTrackerComponent>())
		{
			// Example: +200 for kill
			StatTracker->AddScore(200);
			StatTracker->AddMoney(200);
			StatTracker->AddKill();  // You can create AddKill() method too
		}
	}

	// Optional VFX
	if (DeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
	}

	// Disable AI, collision, etc.
	DetachFromControllerPendingDestroy();
	SetActorEnableCollision(false);
	SetLifeSpan(2.0f); // Clean up actor after delay
}

float AMerc_BaseEnemy::GetDamageMultiplierFromComponent_Implementation(UPrimitiveComponent* HitComponent) const
{
	for (const FDamageZone& Zone : DamageZones)
	{
		if (Zone.Collider == HitComponent)
		{
			return Zone.DamageMultiplier;
		}
	}
	return 1.0f;  // Default no-multiplier
}

void AMerc_BaseEnemy::InitCapsuleColliders()
{
	// Example implementation — override in child class
/*
	UE_LOG(LogTemp, Warning, TEXT("InitCapsuleColliders called"));

	HeadCollider = CreateZoneCollider(TEXT("HeadCollider"), TEXT("head"), FVector(10.f, 15.f, 0), 2.0f);
	BodyCollider = CreateZoneCollider(TEXT("BodyCollider"), TEXT("Spine2"), FVector(20.f, 30.f, 0), 1.0f);

	UE_LOG(LogTemp, Warning, TEXT("Final DamageZones.Num = %d"), DamageZones.Num());
*/
}

UCapsuleComponent* AMerc_BaseEnemy::CreateZoneCollider(FName Name, FName Bone, FVector Size, float Multiplier)
{
	UCapsuleComponent* Capsule = CreateDefaultSubobject<UCapsuleComponent>(Name);
	Capsule->SetupAttachment(GetMesh(), Bone);
	Capsule->InitCapsuleSize(Size.X, Size.Y);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	FDamageZone Zone;
	Zone.Collider = Capsule;
	Zone.DamageMultiplier = Multiplier;
	//DamageZones.Add(Zone);

	return Capsule;
}

void AMerc_BaseEnemy::AddingDamageZones()
{
	// Example implementation — override in child class
/*
	DamageZones.Empty();
	DamageZones.Add({ HeadCollider, HeadDamageMultiplier });
	DamageZones.Add({ BodyCollider, BodyDamageMultiplier });

	UE_LOG(LogTemp, Warning, TEXT("Final DamageZones.Num = %d"), DamageZones.Num());

	for (const FDamageZone& Zone : DamageZones)
	{
		if (Zone.Collider)
		{
			UE_LOG(LogTemp, Warning, TEXT("Zone Collider: %s | %p"), *Zone.Collider->GetName(), Zone.Collider);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("A DamageZone was added with nullptr collider!"));
		}
	}
*/
}

