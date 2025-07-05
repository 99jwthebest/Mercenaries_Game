// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Merc_Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AMerc_Zombie::AMerc_Zombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitCapsuleColliders();
}

// Called when the game starts or when spawned
void AMerc_Zombie::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	AddingDamageZones();
}

// Called every frame
void AMerc_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


float AMerc_Zombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = FMath::Min(CurrentHealth, DamageAmount);
	CurrentHealth -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Zombie took %f damage. Remaining HP: %f"), DamageApplied, CurrentHealth);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return DamageApplied;
}

void AMerc_Zombie::ResetAttackCooldown()
{
	bCanAttack = true;
	UE_LOG(LogTemp, Warning, TEXT("Zombie cooldown reset. Can attack again."));
}

void AMerc_Zombie::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie died!"));

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

float AMerc_Zombie::GetDamageMultiplierFromComponent(UPrimitiveComponent* HitComp) const
{
	if (!HitComp)
	{
		UE_LOG(LogTemp, Error, TEXT("HitComp is null!"));
		return 1.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("HitComp pointer: %p | name: %s | class: %s"),
		HitComp, *HitComp->GetName(), *HitComp->GetClass()->GetName());

	for (const FDamageZone& Zone : DamageZones)
	{
		if (!Zone.Collider)
		{
			UE_LOG(LogTemp, Error, TEXT("Found a damage zone with a null collider!"));
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("Comparing to Zone Collider pointer: %p | name: %s | class: %s"),
			Zone.Collider, *Zone.Collider->GetName(), *Zone.Collider->GetClass()->GetName());

		if (HitComp == Zone.Collider)
		{
			UE_LOG(LogTemp, Log, TEXT("Matched by pointer!"));
			return Zone.DamageMultiplier;
		}

		if (HitComp->GetFName() == Zone.Collider->GetFName())
		{
			UE_LOG(LogTemp, Log, TEXT("Matched by name!"));
			return Zone.DamageMultiplier;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No damage zone matched. Using default multiplier."));
	return 1.0f;

}

float AMerc_Zombie::GetDamageMultiplierFromComponent_Implementation(UPrimitiveComponent* HitComponent) const
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

void AMerc_Zombie::InitCapsuleColliders()
{
	UE_LOG(LogTemp, Warning, TEXT("InitCapsuleColliders called"));

	HeadCollider = CreateZoneCollider(TEXT("HeadCollider"), TEXT("head"), FVector(10.f, 15.f, 0), 2.0f);
	BodyCollider = CreateZoneCollider(TEXT("BodyCollider"), TEXT("spine_03"), FVector(20.f, 30.f, 0), 1.0f);
	LeftArmCollider = CreateZoneCollider(TEXT("LeftArmCollider"), TEXT("upperarm_l"), FVector(8.f, 20.f, 0), 0.8f);
	RightArmCollider = CreateZoneCollider(TEXT("RightArmCollider"), TEXT("upperarm_r"), FVector(8.f, 20.f, 0), 0.8f);
	LeftLegCollider = CreateZoneCollider(TEXT("LeftLegCollider"), TEXT("thigh_l"), FVector(10.f, 25.f, 0), 0.7f);
	RightLegCollider = CreateZoneCollider(TEXT("RightLegCollider"), TEXT("thigh_r"), FVector(10.f, 25.f, 0), 0.7f);

	UE_LOG(LogTemp, Warning, TEXT("Final DamageZones.Num = %d"), DamageZones.Num());

}

UCapsuleComponent* AMerc_Zombie::CreateZoneCollider(FName Name, FName Bone, FVector Size, float Multiplier)
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

void AMerc_Zombie::AddingDamageZones()
{
	DamageZones.Empty();
	DamageZones.Add({ HeadCollider, HeadDamageMultiplier });
	DamageZones.Add({ BodyCollider, BodyDamageMultiplier });
	DamageZones.Add({ LeftArmCollider, LeftArmDamageMultiplier });
	DamageZones.Add({ RightArmCollider, RightArmDamageMultiplier });
	DamageZones.Add({ LeftLegCollider, LeftLegDamageMultiplier });
	DamageZones.Add({ RightLegCollider, RightLegDamageMultiplier });

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
}
