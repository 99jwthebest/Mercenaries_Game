// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs/DamageZoneTypes.h"
#include "Interfaces/IHitDamageable.h"
#include "Merc_Zombie.generated.h"

UCLASS()
class AMerc_Zombie : public ACharacter, public IHitDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMerc_Zombie();

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

	void ResetAttackCooldown();

	void Die();

	// Example helper
	float GetDamageMultiplierFromComponent(UPrimitiveComponent* HitComp) const;

	virtual float GetDamageMultiplierFromComponent_Implementation(UPrimitiveComponent* HitComponent) const override;

protected:

	void InitCapsuleColliders();
	UCapsuleComponent* CreateZoneCollider(FName Name, FName Bone, FVector Size, float Multiplier);
	void AddingDamageZones();
};
