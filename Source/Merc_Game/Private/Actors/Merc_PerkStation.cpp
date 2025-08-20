// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_PerkStation.h"
#include "DataAssets/PerkDataAsset.h"


AMerc_PerkStation::AMerc_PerkStation()
{
	ItemType = EItemType::Perk;
	ItemData = PerkData; // Will be set in editor
}

void AMerc_PerkStation::OnBeginInteract(AActor* Interactor)
{
	if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return;

	LastBuyer = Interactor;
	IBuyer::Execute_SetNearbyInteractable(Interactor, this);

	// Always show perk name and cost
	if (PerkData)
	{
		UE_LOG(LogTemp, Error, TEXT("Juggernaug!!!!!!"));

		IBuyer::Execute_ShowBuyPrompt(Interactor, this); // Pass the station itself
	}
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
	if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()) || !PerkData)
		return;

	if (TryPurchase(Interactor))
	{
		UE_LOG(LogTemp, Log, TEXT("Perk purchased: %s"), *PerkData->DisplayName);

		IBuyer::Execute_ShowBuyPrompt(Interactor, this); // Pass the station itself
	}
}

bool AMerc_PerkStation::TryPurchase(AActor* BuyerActor)
{
	if (!BuyerActor || !BuyerActor->GetClass()->ImplementsInterface(UBuyer::StaticClass()) || !PerkData)
		return false;

	// If they already have this perk, don't buy again
	if (IBuyer::Execute_HasItem(BuyerActor, EItemType::Perk, PerkData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player already owns perk: %s"), *PerkData->DisplayName);
		return false;
	}

	// Try to purchase the perk
	return IBuyer::Execute_TryPurchase(BuyerActor, EItemType::Perk, PerkData, PerkData->Cost);
}

FString AMerc_PerkStation::GetInteractionPrompt(AActor* Interactor) const
{
	if (!Interactor)
		return PrimaryPrompt;

	if (!Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return PrimaryPrompt;

	// Check if the player already has the perk
	const bool bHasPerk = IBuyer::Execute_HasItem(Interactor, EItemType::Perk, PerkData);

	if (bHasPerk)
	{
		// If you want a second prompt, use it; otherwise, hide the prompt
		return !SecondaryPrompt.IsEmpty() ? SecondaryPrompt : FString("");
	}

	// Default prompt (buy the perk)
	return PrimaryPrompt;
}