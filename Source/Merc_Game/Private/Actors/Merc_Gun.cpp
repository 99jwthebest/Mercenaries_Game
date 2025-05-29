// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_Gun.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"


// Sets default values
AMerc_Gun::AMerc_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	Mesh->SetupAttachment(Root);

	// Create MuzzlePoint and attach to the pistol mesh
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(Root);
}


void AMerc_Gun::PullTrigger()
{
	UE_LOG(LogTemp, Error, TEXT("You've Been Shot!!"));
	if (MuzzleFlashEffect && MuzzleSound)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, MuzzleLocation->GetComponentTransform());
	}
	if (MuzzleSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("RightHand"));
		UE_LOG(LogTemp, Error, TEXT("You've Been Shot WITH SOUNDSSSSS!!"));

	}

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		if (ImpactEffect && ImpactSound)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

	//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 3, FColor::Red, true);
}

bool AMerc_Gun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AMerc_Gun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

// Called when the game starts or when spawned
void AMerc_Gun::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMerc_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

