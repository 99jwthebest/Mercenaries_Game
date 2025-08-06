// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_WeaponDisplay.h"
#include "Merc_Gun.h"


// Sets default values
AMerc_WeaponDisplay::AMerc_WeaponDisplay()
{
    ItemType = EItemType::Weapon;
    ItemData = WeaponClass; // your weapon TSubclassOf
    BaseCost = WeaponCost;
    AltCost = AmmoCost;
}

void AMerc_WeaponDisplay::OnBeginInteract(AActor* Interactor)
{
     if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass())) 
        return;

    LastBuyer = Interactor;
    IBuyer::Execute_SetNearbyInteractable(Interactor, this);

    // Check if the player already owns this weapon
    const bool bOwnsWeapon = IBuyer::Execute_HasItem(Interactor, EItemType::Weapon, WeaponClass);
    IBuyer::Execute_ShowBuyPrompt(Interactor, WeaponName, bOwnsWeapon ? AmmoCost : WeaponCost, bOwnsWeapon);
}

void AMerc_WeaponDisplay::OnEndInteract(AActor* Interactor)
{
    if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return;

    IBuyer::Execute_SetNearbyInteractable(Interactor, nullptr);
    IBuyer::Execute_HideBuyPrompt(Interactor);
}

void AMerc_WeaponDisplay::Interact(AActor* Interactor)
{
    if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return;

    if (TryPurchase(Interactor))
    {
        UE_LOG(LogTemp, Log, TEXT("Purchase successful: %s"), *GetName());

        const bool bOwnsWeapon = IBuyer::Execute_HasItem(Interactor, EItemType::Weapon, WeaponClass);
        IBuyer::Execute_ShowBuyPrompt(Interactor, WeaponName, bOwnsWeapon ? AmmoCost : WeaponCost, bOwnsWeapon);
    }
}

bool AMerc_WeaponDisplay::TryPurchase(AActor* BuyerActor)
{
    if (!BuyerActor || !BuyerActor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return false;

    const bool bOwnsWeapon = IBuyer::Execute_HasItem(BuyerActor, EItemType::Weapon, WeaponClass);
    if (bOwnsWeapon)
    {
        return IBuyer::Execute_TryPurchase(BuyerActor, EItemType::Ammo, WeaponClass, AmmoCost);
    }
    else
    {
        return IBuyer::Execute_TryPurchase(BuyerActor, ItemType, WeaponClass, WeaponCost);
    }
}

