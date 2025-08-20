// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Merc_BaseInteractable.h"
#include "Merc_PerkStation.generated.h"

class UPerkDataAsset;

UCLASS()
class AMerc_PerkStation : public AMerc_BaseInteractable
{
	GENERATED_BODY()
	
public:
	AMerc_PerkStation();

protected:
	// The perk this station sells
	UPROPERTY(EditAnywhere, Category = "Perk")
	UPerkDataAsset* PerkData;

	UPROPERTY()
	TWeakObjectPtr<AActor> LastBuyer;

protected:
	virtual void OnBeginInteract(AActor* Interactor) override;
	virtual void OnEndInteract(AActor* Interactor) override;

public:
	virtual void Interact(AActor* Interactor) override;
	virtual bool TryPurchase(AActor* BuyerActor) override;

	virtual FString GetInteractionPrompt(AActor* Interactor) const override;

	//FName GetPerkID() const { return PerkID; }
	//int32 GetPerkCost() const { return PerkCost; }
	//FString GetPerkName() const { return PerkName; }
};
