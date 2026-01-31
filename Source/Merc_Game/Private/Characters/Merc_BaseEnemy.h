// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs/DamageZoneTypes.h"
#include "Interfaces/IHitDamageable.h"
#include "Merc_BaseEnemy.generated.h"

// Delegate signature: int32 CurrentAmmo, int32 MaxAmmo
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, AMerc_BaseEnemy*, EnemyDied);

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

	UPROPERTY(EditAnywhere, Category = "Death")
	UParticleSystem* DeathEffect; // optional: blood/explosion FX

	UPROPERTY()
	TArray<FDamageZone> DamageZones;

	UPROPERTY()
	AController* LastInstigator = nullptr;
	APawn* TargetPlayer;

	// Optional state gate if you want to stop AI/actions during stun
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsStunned = false;

	FTimerHandle StunTimer;

public:

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackCooldown = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20.f;

	UPROPERTY()
	bool bCanAttack = true;

	FTimerHandle AttackCooldownTimer;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeath OnEnemyDeath;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ResetAttackCooldown();

	virtual void Die();

	virtual float GetDamageMultiplierFromComponent_Implementation(UPrimitiveComponent* HitComponent) const override;

	virtual void ApplyHit_Implementation(const FHitSpec& Spec) override;

	void NotifyEnemyDead();

protected:

	/**
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

	// Child classes can opt in/out
	virtual bool CanBeKnockedBack() const { return false; }
	virtual bool CanBeStunned() const { return true; }

	// Called by ApplyHit after damage is applied (unless dead)
	virtual void HandleHitReaction(const FHitSpec& Spec);

	// Helpers
	void FaceInstigatorIfNeeded(const FHitSpec& Spec);
	void ApplyKnockbackIfAllowed(const FHitSpec& Spec);
	void ApplyStunIfAllowed(float Duration);

	void ClearStun();

};
