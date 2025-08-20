// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_BaseInteractable.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMerc_BaseInteractable::AMerc_BaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Trigger
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Mesh
	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(RootComponent);

}

void AMerc_BaseInteractable::BeginPlay()
{
	// Bind overlap events
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMerc_BaseInteractable::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMerc_BaseInteractable::OnOverlapEnd);
}

void AMerc_BaseInteractable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	bPlayerInRange = true;
	CurrentInteractor = OtherActor;
	LastInteractor = OtherActor;

	OnBeginInteract(OtherActor);
}

void AMerc_BaseInteractable::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LastInteractor.Get() == OtherActor)
	{
		bPlayerInRange = false;
		OnEndInteract(OtherActor);
		CurrentInteractor = nullptr;
		LastInteractor = nullptr;
	}
}

void AMerc_BaseInteractable::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("%s interacted with %s"),
		Interactor ? *Interactor->GetName() : TEXT("Unknown"),
		*GetName());
}

bool AMerc_BaseInteractable::TryPurchase(AActor* BuyerActor)
{
	if (!BuyerActor || !BuyerActor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return false;

	return IBuyer::Execute_TryPurchase(BuyerActor, ItemType, ItemData, BaseCost);
}

FString AMerc_BaseInteractable::GetInteractionPrompt(AActor* Interactor) const
{
	if (!Interactor)
		return PrimaryPrompt;

	if (!Interactor->GetClass()->ImplementsInterface(UBuyer::StaticClass()))
		return PrimaryPrompt;

	const bool bAlreadyHasItem = IBuyer::Execute_HasItem(Interactor, ItemType, ItemData);

	if (bAlreadyHasItem)
	{
		if (!SecondaryPrompt.IsEmpty())
			return SecondaryPrompt;
		else
			return ""; // Hide prompt if nothing to show
	}

	UE_LOG(LogTemp, Error, TEXT("BuggerNaugh Is glicthing!!!"));

	return PrimaryPrompt;
}

