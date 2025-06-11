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
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("RightHand"));
		UE_LOG(LogTemp, Error, TEXT("You've Been Shot WITH SOUNDSSSSS!!"));
	}
	if (RecoilShake)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(RecoilShake);

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

	// Add bullet spread
	const FVector ShootDir = Rotation.Vector();
	const float SpreadRad = GetBulletSpreadRadians();
	ShotDirection = FMath::VRandCone(ShootDir, SpreadRad); // apply spread

	const FVector End = Location + ShotDirection * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_GameTraceChannel1, Params);
}

AController* AMerc_Gun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

EFireMode AMerc_Gun::GetFireMode() const
{
	return FireMode;
}

float AMerc_Gun::GetBulletSpreadRadians() const
{
	return FMath::DegreesToRadians(BulletSpreadDegrees);
}


void AMerc_Gun::StartFiring()
{
	if (bIsReloading || CurrentAmmo <= 0 || bIsFiring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire: Reloading (%d), Ammo = %d, Already firing = %d"), bIsReloading, CurrentAmmo, bIsFiring);
		return;
	}

	if (!bCanFire)
	{
		// Player tried to fire during cooldown — buffer it
		bWantsToFire = true;
		return;
	}

	bIsFiring = true;
	UE_LOG(LogTemp, Log, TEXT("StartFiring: FireMode = %d"), static_cast<int32>(FireMode));

	switch (FireMode)
	{
		case EFireMode::SemiAuto:
			HandleFiring();
			// Add cooldown before next shot is allowed
			GetWorld()->GetTimerManager().SetTimer(SemiAutoFireRateTimer, this, &AMerc_Gun::ResetBurst, FireRate, false);
			break;

		case EFireMode::FullAuto:
			HandleFiring();
			bCanFire = false;
			GetWorld()->GetTimerManager().SetTimer(FullAutoCooldownTimer, this, &AMerc_Gun::ResettingFullAutoFire, FireRate, false);
			GetWorld()->GetTimerManager().SetTimer(FullAutoFireRateTimer, this, &AMerc_Gun::HandleFiring, FireRate, true);
			UE_LOG(LogTemp, Log, TEXT("FullAuto: Timer set with FireRate = %f"), FireRate);
			break;

		case EFireMode::Burst:
			BurstShotsRemaining = BurstShotsPerCycle;
			UE_LOG(LogTemp, Log, TEXT("Burst: Starting burst with %d shots"), BurstShotsRemaining);
			HandleBurstFiring();
			break;
	}
}

void AMerc_Gun::HandleFiring()
{
	if (CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ammo left!"));
		StopFiring();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Firing! Current Ammo = %d"), CurrentAmmo);
	PullTrigger();
	CurrentAmmo--;
	UE_LOG(LogTemp, Log, TEXT("Ammo after shot = %d"), CurrentAmmo);
	ApplyRecoil();
	NotifyAmmoChanged();
}

void AMerc_Gun::HandleBurstFiring()
{
	HandleFiring();
	BurstShotsRemaining--;
	UE_LOG(LogTemp, Log, TEXT("BurstFiring: Shots remaining = %d"), BurstShotsRemaining);

	if (BurstShotsRemaining <= 0 || CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("BurstFiring complete. Starting cooldown for %f seconds."), BurstCycleRate);
		GetWorld()->GetTimerManager().SetTimer(BurstCooldownTimer, this, &AMerc_Gun::ResetBurst, BurstCycleRate, false);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(BurstShotTimer, this, &AMerc_Gun::HandleBurstFiring, BurstShotRate, false);
	UE_LOG(LogTemp, Log, TEXT("Next burst shot in %f seconds."), BurstShotRate);
}

void AMerc_Gun::ResetBurst()
{
	bIsFiring = false;
	UE_LOG(LogTemp, Log, TEXT("Burst reset complete. Gun ready."));
}

void AMerc_Gun::StopFiring()
{
	bIsFiring = false;

	GetWorld()->GetTimerManager().ClearTimer(FullAutoFireRateTimer);
	GetWorld()->GetTimerManager().ClearTimer(BurstShotTimer);
	GetWorld()->GetTimerManager().ClearTimer(BurstCooldownTimer);
	 // ****** Maybe this might help to stop bursting if something happens to player

	UE_LOG(LogTemp, Log, TEXT("Stopped firing. Timers cleared."));
}

void AMerc_Gun::StartReload()
{
	if (bIsReloading || CurrentAmmo == MaxAmmo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reload skipped. Already reloading or full ammo."));
		return;
	}

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("Reloading started..."));

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMerc_Gun::FinishReload, ReloadDuration, false);
}

void AMerc_Gun::FinishReload()
{
	bIsReloading = false;
	CurrentAmmo = MaxAmmo;
	UE_LOG(LogTemp, Log, TEXT("Reload complete. Ammo = %d"), CurrentAmmo);
	NotifyAmmoChanged();
}

void AMerc_Gun::CancelReload()
{
	if (bIsReloading)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		bIsReloading = false;
		UE_LOG(LogTemp, Warning, TEXT("Reload cancelled!"));
	}
}

void AMerc_Gun::ResettingFullAutoFire()
{
	bCanFire = true;

	// If the player tapped during cooldown, fire immediately
	if (bWantsToFire)
	{
		bWantsToFire = false;
		StartFiring();  // Will immediately consume input and start again
		UE_LOG(LogTemp, Log, TEXT("Fire cooldown reset. Can fire again."));
	}
}

void AMerc_Gun::ApplyRecoil()
{
	APlayerController* PC = Cast<APlayerController>(GetOwnerController());
	if (PC)
	{
		float PitchRecoil = FMath::RandRange(RecoilPitchMin, RecoilPitchMax); // e.g., 1.0f to 2.0f
		float YawRecoil = FMath::RandRange(-RecoilYawMax, RecoilYawMax);      // e.g., -1.0f to 1.0f

		RecoilAccumulated += FRotator(PitchRecoil, YawRecoil, 0.f); // accumulate upward and sideways

		FRotator NewRotation = PC->GetControlRotation();
		NewRotation += FRotator(PitchRecoil, YawRecoil, 0.f);
		PC->SetControlRotation(NewRotation);
	}
}

int AMerc_Gun::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int AMerc_Gun::GetMaxAmmo()
{
	return MaxAmmo;
}

UTexture2D* AMerc_Gun::GetGunIcon()
{
	return GunIcon;
}

// Called when the game starts or when spawned
void AMerc_Gun::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;


}
// Called every frame
void AMerc_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = Cast<APlayerController>(GetOwnerController());
	if (PC && !RecoilAccumulated.IsNearlyZero())
	{
		FRotator CurrentRot = PC->GetControlRotation();
		FRotator RecoveryStep = RecoilRecoverySpeed * DeltaTime;

		// Recover pitch
		if (RecoilAccumulated.Pitch > 0.f)
		{
			float Delta = FMath::Min(RecoilAccumulated.Pitch, RecoveryStep.Pitch);
			RecoilAccumulated.Pitch -= Delta;
			CurrentRot.Pitch -= Delta;
		}

		// Recover yaw
		if (FMath::Abs(RecoilAccumulated.Yaw) > 0.f)
		{
			float Delta = FMath::Min(FMath::Abs(RecoilAccumulated.Yaw), RecoveryStep.Yaw);
			float Sign = FMath::Sign(RecoilAccumulated.Yaw);
			RecoilAccumulated.Yaw -= Delta * Sign;
			CurrentRot.Yaw -= Delta * Sign;
		}

		PC->SetControlRotation(CurrentRot);
	}
}

void AMerc_Gun::NotifyAmmoChanged()
{
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}

