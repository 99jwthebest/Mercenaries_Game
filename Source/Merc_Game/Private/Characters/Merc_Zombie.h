// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/Merc_BaseEnemy.h"
#include "Merc_Zombie.generated.h"

UCLASS()
class AMerc_Zombie : public AMerc_BaseEnemy
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


protected:
	
	virtual void InitCapsuleColliders() override;
	virtual void AddingDamageZones() override;
};
