// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Merc_BaseInteractable.h"
#include "Merc_PerkStation.generated.h"

/**
 * 
 */
UCLASS()
class AMerc_PerkStation : public AMerc_BaseInteractable
{
	GENERATED_BODY()
	
public:
	AMerc_PerkStation();

protected:
	/** ID or class reference for this perk */
	UPROPERTY(EditAnywhere, Category = "Perk")
	FName PerkID; // "Juggernog", "SpeedCola", etc.

	UPROPERTY(EditAnywhere, Category = "Perk")
	FString PerkName = "Perk";

	UPROPERTY(EditAnywhere, Category = "Perk")
	int32 PerkCost = 2000;

	UPROPERTY()
	TWeakObjectPtr<AActor> LastBuyer;

protected:
	virtual void OnBeginInteract(AActor* Interactor) override;
	virtual void OnEndInteract(AActor* Interactor) override;

public:
	virtual void Interact(AActor* Interactor) override;
	virtual bool TryPurchase(AActor* BuyerActor) override;

	FName GetPerkID() const { return PerkID; }
	int32 GetPerkCost() const { return PerkCost; }
	FString GetPerkName() const { return PerkName; }
};
