// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Merc_Door.h"

AMerc_Door::AMerc_Door()
{
    // Hinge for pivot
    DoorHinge = CreateDefaultSubobject<USceneComponent>(TEXT("DoorHinge"));
    DoorHinge->SetupAttachment(RootComponent);


	ItemType = EItemType::Door;
	ItemData = nullptr;
    BaseCost = DoorCost;
}

void AMerc_Door::BeginPlay()
{
    Super::BeginPlay();

    if (DisplayMesh && DoorHinge)
    {
        // Attach the mesh to the hinge at runtime
        DisplayMesh->AttachToComponent(DoorHinge, FAttachmentTransformRules::KeepRelativeTransform);

        // Offset the mesh so it rotates correctly (adjust Y for your mesh width)
       // DisplayMesh->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
    }

}

void AMerc_Door::OnBeginInteract(AActor* Interactor)
{
    if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return;

    IBuyer::Execute_SetNearbyInteractable(Interactor, this);
    IBuyer::Execute_ShowBuyPrompt(Interactor, this);
}

void AMerc_Door::OnEndInteract(AActor* Interactor)
{
    if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return;

    IBuyer::Execute_SetNearbyInteractable(Interactor, nullptr);
    IBuyer::Execute_HideBuyPrompt(Interactor);
}

void AMerc_Door::Interact(AActor* Interactor)
{
    if (!Interactor || !Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return;

    if (TryPurchase(Interactor))
    {
        UE_LOG(LogTemp, Log, TEXT("Door purchased: %s"), *GetName());

        // Refresh prompt so player sees "Already Owned" or hides it
        IBuyer::Execute_ShowBuyPrompt(Interactor, this);

        // Optional: unlock or open the door here
        OpenDoor();
    }
}

bool AMerc_Door::TryPurchase(AActor* BuyerActor)
{
    if (!BuyerActor || !BuyerActor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return false;

    // If player already bought this door, don't buy again
    if (IBuyer::Execute_HasItem(BuyerActor, EItemType::Door, this))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player already owns door: %s"), *GetName());
        return false;
    }

    // Attempt purchase
    return IBuyer::Execute_TryPurchase(BuyerActor, EItemType::Door, this, DoorCost);
}

FString AMerc_Door::GetInteractionPrompt(AActor* Interactor) const
{
    if (!Interactor)
        return PrimaryPrompt;

    if (!Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
        return PrimaryPrompt;

    const bool bHasDoor = IBuyer::Execute_HasItem(Interactor, EItemType::Door, const_cast<AMerc_Door*>(this));

    if (bHasDoor)
    {
        // Show secondary prompt if set, otherwise hide
        return !SecondaryPrompt.IsEmpty() ? SecondaryPrompt : FString("");
    }

    // Default prompt to buy the door
    return PrimaryPrompt;
}