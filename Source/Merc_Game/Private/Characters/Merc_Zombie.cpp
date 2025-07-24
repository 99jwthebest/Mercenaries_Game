// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Merc_Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatTrackerComponent.h"


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

	AddingDamageZones();
}

// Called every frame
void AMerc_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMerc_Zombie::InitCapsuleColliders()
{
	UE_LOG(LogTemp, Warning, TEXT("InitCapsuleColliders called"));

	HeadCollider = CreateZoneCollider(TEXT("HeadCollider"), TEXT("head"), FVector(10.f, 15.f, 0), 2.0f);
	BodyCollider = CreateZoneCollider(TEXT("BodyCollider"), TEXT("Spine2"), FVector(20.f, 30.f, 0), 1.0f);
	LeftArmCollider = CreateZoneCollider(TEXT("LeftArmCollider"), TEXT("LeftArm"), FVector(8.f, 20.f, 0), 0.8f);
	RightArmCollider = CreateZoneCollider(TEXT("RightArmCollider"), TEXT("RightArm"), FVector(8.f, 20.f, 0), 0.8f);
	LeftLegCollider = CreateZoneCollider(TEXT("LeftLegCollider"), TEXT("LeftUpLeg"), FVector(10.f, 25.f, 0), 0.7f);
	RightLegCollider = CreateZoneCollider(TEXT("RightLegCollider"), TEXT("RightUpLeg"), FVector(10.f, 25.f, 0), 0.7f);

	UE_LOG(LogTemp, Warning, TEXT("Final DamageZones.Num = %d"), DamageZones.Num());

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
