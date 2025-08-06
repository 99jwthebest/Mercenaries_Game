// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_WeaponDisplay.h"
#include "Merc_Gun.h"
#include "Interfaces/IWeaponBuyer.h"


// Sets default values
AMerc_WeaponDisplay::AMerc_WeaponDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMerc_WeaponDisplay::OnBeginInteract(AActor* Interactor)
{
    if (!Interactor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass())) return;

    LastBuyer = Interactor;

    IWeaponBuyer::Execute_SetNearbyInteractable(Interactor, this);

    const bool bOwnsWeapon = IWeaponBuyer::Execute_HasWeapon(Interactor, WeaponClass);
    if (bOwnsWeapon)
    {
        IWeaponBuyer::Execute_ShowWeaponBuyPrompt(Interactor, WeaponName, AmmoCost, true);
    }
    else
    {
        IWeaponBuyer::Execute_ShowWeaponBuyPrompt(Interactor, WeaponName, WeaponCost, false);
    }
}

void AMerc_WeaponDisplay::OnEndInteract(AActor* Interactor)
{
    if (!Interactor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass())) return;

    IWeaponBuyer::Execute_SetNearbyInteractable(Interactor, nullptr);
    IWeaponBuyer::Execute_HideWeaponBuyPrompt(Interactor);
}

bool AMerc_WeaponDisplay::TryPurchase(AActor* BuyerActor)
{
    if (!BuyerActor || !WeaponClass || !BuyerActor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass()))
        return false;

    if (IWeaponBuyer::Execute_HasWeapon(BuyerActor, WeaponClass))
    {
        AMerc_Gun* OwnedGun = IWeaponBuyer::Execute_GetWeaponByClass(BuyerActor, WeaponClass);
        if (OwnedGun && OwnedGun->CanRefillAmmo())
        {
            if (IWeaponBuyer::Execute_TryRefillAmmo(BuyerActor, WeaponClass, AmmoCost))
            {
                LastBuyer = BuyerActor;
                return true;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Ammo is already full. Cannot refill."));
        }

        return false;
    }

    if (IWeaponBuyer::Execute_TryBuyWeapon(BuyerActor, WeaponClass, WeaponCost))
    {
        LastBuyer = BuyerActor;
        return true;
    }

    return false;
}

void AMerc_WeaponDisplay::Interact(AActor* Interactor)
{
    if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass()))
        return;

    if (TryPurchase(Interactor))
    {
        UE_LOG(LogTemp, Log, TEXT("Purchase successful: %s"), *GetName());

        const bool bOwnsWeapon = IWeaponBuyer::Execute_HasWeapon(Interactor, WeaponClass);
        if (bOwnsWeapon)
        {
            IWeaponBuyer::Execute_ShowWeaponBuyPrompt(Interactor, WeaponName, AmmoCost, true);
        }
        else
        {
            IWeaponBuyer::Execute_ShowWeaponBuyPrompt(Interactor, WeaponName, WeaponCost, false);
        }
    }
}

