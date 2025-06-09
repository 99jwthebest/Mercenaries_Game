// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Merc_Gun.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAuto,
	FullAuto,
	Burst,
	Bolt
};


UCLASS()
class AMerc_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMerc_Gun();

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	UFUNCTION(BlueprintPure, Category = "Weapon")
	EFireMode GetFireMode() const;
	// Internal use only — not exposed to Blueprint
	float GetBulletSpreadRadians() const;

protected:

	virtual void PullTrigger();
	void HandleFiring();
	void HandleBurstFiring();

public:

	void StartFiring();
	void ResetBurst();
	void StopFiring();
	void StartReload();
	void FinishReload();
	void ResettingFullAutoFire();
	void ApplyRecoil();
	

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float MaxRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	EFireMode FireMode = EFireMode::SemiAuto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.1f;

	FTimerHandle FullAutoFireRateTimer;
	FTimerHandle FullAutoCooldownTimer;
	bool bIsFiring = false;
	bool bResettingFullAutoFire = false;
	bool bCanFire = true;
	bool bWantsToFire = false;

	FTimerHandle SemiAutoFireRateTimer;

	// Timers
	FTimerHandle BurstShotTimer;
	FTimerHandle BurstCooldownTimer;

	// Burst settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 BurstShotsPerCycle = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float BurstShotRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float BurstCycleRate = 0.5f;

	int32 BurstShotsRemaining = 0;

	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> RecoilShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo = 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float ReloadDuration = 2.0f;
	bool bIsReloading = false;
	FTimerHandle ReloadTimerHandle;

	// This is the user-friendly version exposed to Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Accuracy", meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "10.0", AllowPrivateAccess = "true"))
	float BulletSpreadDegrees = 2.5f;

	// Recoil - Vertical
	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilPitchMin = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilPitchMax = 2.0f;

	// Recoil - Horizontal
	UPROPERTY(EditAnywhere, Category = "Weapon|Recoil")
	float RecoilYawMax = 1.0f;

	FRotator RecoilAccumulated;
	// Recoil - Recovery
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Recoil", meta = (ClampMin = "0.0", ClampMax = "30.0", UIMin = "1.0", UIMax = "10.0", AllowPrivateAccess = "true"))
	FRotator RecoilRecoverySpeed = FRotator(10.f, 5.f, 0.f); // degrees per second


};
