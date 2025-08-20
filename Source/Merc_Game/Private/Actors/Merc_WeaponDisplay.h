// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Merc_BaseInteractable.h"
#include "Merc_WeaponDisplay.generated.h"

class AMerc_Gun;

UCLASS()
class AMerc_WeaponDisplay : public AMerc_BaseInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMerc_WeaponDisplay();

protected:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AMerc_Gun> WeaponClass;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FString WeaponName = "Weapon";

    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 WeaponCost = 500;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 AmmoCost = 250;

    UPROPERTY()
    TWeakObjectPtr<AActor> LastBuyer;

protected:
    virtual void OnBeginInteract(AActor* Interactor) override;
    virtual void OnEndInteract(AActor* Interactor) override;

public:
    // Player pressed Interact
    virtual void Interact(AActor* Interactor) override;
    virtual bool TryPurchase(AActor* BuyerActor) override;
    virtual FString GetInteractionPrompt(AActor* Interactor) const override;



    int32 GetWeaponCost() const { return WeaponCost; }
    int32 GetWeaponAmmoCost() const { return AmmoCost; }
    TSubclassOf<AMerc_Gun> GetWeaponClass() const { return WeaponClass; }
    FString GetWeaponName() const { return WeaponName; }

};
