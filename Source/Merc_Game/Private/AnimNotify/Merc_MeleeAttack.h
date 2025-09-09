// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Merc_MeleeAttack.generated.h"

/**
 * 
 */
class UMerc_MeleeComponent;
UCLASS()
class UMerc_MeleeAttack : public UAnimNotify
{
	GENERATED_BODY()
	

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


private:

	ACharacter* PlayerCharacter;
	UMerc_MeleeComponent* MeleeComp;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float End;
	UPROPERTY(EditAnywhere, Category = "Damage")
	float Radius;
	UPROPERTY(EditAnywhere, Category = "Damage")
	float AmountOfDamage;
	
};
