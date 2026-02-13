// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Merc_MeleeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMerc_MeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMerc_MeleeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PerformMeleeAttack();
	void MeleeTrace();
	FVector MeleeTraceCalcuation();

private:
	void ResetAttackCooldown();

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	// Damage amount for the melee attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float MeleeDamage = 50.0f;

	// Range of the melee attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Trace")
	float MeleeRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Trace")
	float MeleeRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Knockback")
	float KnockbackStrength = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Knockback")
	float UpwardBoost = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|Stun")
	float StunTime = 1.0f;


	 // Optional: attack cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float AttackCooldown = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	UAnimMontage* MeleeAttackMontage;

private: 
	bool bCanAttack = true;
	FTimerHandle AttackCooldownTimerHandle;
	FVector MeleeForwardVector;

};
