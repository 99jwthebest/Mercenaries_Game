// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_PerkStation.h"

AMerc_PerkStation::AMerc_PerkStation()
{
	ItemType = EItemType::Perk;
	ItemData = &PerkID; // You could later pass a Perk Data Asset instead of nullptr
	BaseCost = PerkCost;
}

void AMerc_PerkStation::OnBeginInteract(AActor* Interactor)
{
	if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return;

	LastBuyer = Interactor;
	IBuyer::Execute_SetNearbyInteractable(Interactor, this);

	// Check if player already has this perk
	const bool bHasPerk = IBuyer::Execute_HasItem(Interactor, EItemType::Perk, nullptr /* PerkData or ID */);
	IBuyer::Execute_ShowBuyPrompt(Interactor, PerkName, PerkCost, bHasPerk);
}

void AMerc_PerkStation::OnEndInteract(AActor* Interactor)
{
	if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return;

	IBuyer::Execute_SetNearbyInteractable(Interactor, nullptr);
	IBuyer::Execute_HideBuyPrompt(Interactor);
}

void AMerc_PerkStation::Interact(AActor* Interactor)
{
	if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return;

	if (TryPurchase(Interactor))
	{
		UE_LOG(LogTemp, Log, TEXT("Perk purchased: %s"), *PerkName);
		IBuyer::Execute_ShowBuyPrompt(Interactor, PerkName, PerkCost, true);
	}
}

bool AMerc_PerkStation::TryPurchase(AActor* BuyerActor)
{
	return false;
}
