// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs/DamageZoneTypes.h"
#include "Interfaces/IHitDamageable.h"
#include "Merc_BaseEnemy.generated.h"

UCLASS()
class AMerc_BaseEnemy : public ACharacter, public IHitDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMerc_BaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Health
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float CurrentHealth;
	
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsDead = false;

	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 150.f;

	// Damage range (if needed)
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 100.f;

	// Damage
	//UPROPERTY(EditAnywhere, Category = "Attack")
	//float Damage = 20.f;

	APawn* TargetPlayer;

	UPROPERTY(EditAnywhere, Category = "Death")
	UParticleSystem* DeathEffect; // optional: blood/explosion FX

	UPROPERTY()
	TArray<FDamageZone> DamageZones;


	UPROPERTY()
	UCapsuleComponent* HeadCollider;

	UPROPERTY()
	UCapsuleComponent* BodyCollider;

	UPROPERTY()
	UCapsuleComponent* RightArmCollider;

	UPROPERTY()
	UCapsuleComponent* LeftArmCollider;

	UPROPERTY()
	UCapsuleComponent* LeftLegCollider;

	UPROPERTY()
	UCapsuleComponent* RightLegCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float HeadDamageMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float BodyDamageMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float RightArmDamageMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float LeftArmDamageMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float LeftLegDamageMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float RightLegDamageMultiplier;

	UPROPERTY()
	AController* LastInstigator = nullptr;


public:

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackCooldown = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20.f;

	UPROPERTY()
	bool bCanAttack = true;

	FTimerHandle AttackCooldownTimer;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ResetAttackCooldown();

	virtual void Die();

	virtual float GetDamageMultiplierFromComponent_Implementation(UPrimitiveComponent* HitComponent) const override;

protected:
	/* 
	Override this in child classes to initialize hit capsules on body parts.
	* Should be called in Constructor. 
	*/
	virtual void InitCapsuleColliders();

	/*
	Helper to create a capsule collider with the specified Name, Bone Attachment, Size, and Damage Multiplier.
	* Not intended for Runtime use.
	*/
	UCapsuleComponent* CreateZoneCollider(FName Name, FName Bone, FVector Size, float Multiplier);
	
	/* 
	Override this in child classes to define damage zones (e.g., head, body, arms).
	* Should be called in BeginPlay. 
	*/
	virtual void AddingDamageZones();

};
