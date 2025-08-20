// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/ItemType.h"
#include "Interfaces/IBuyer.h"
#include "Merc_BaseInteractable.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class AMerc_BaseInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMerc_BaseInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// Trigger for detecting player interaction range
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	// Optional mesh for the interactable object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DisplayMesh;

	// Interaction settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FString InteractionName = "Interactable";

	// Is player in range
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bPlayerInRange = false;

	// Who’s in range
	UPROPERTY()
	AActor* CurrentInteractor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> LastInteractor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	EItemType ItemType;
	UObject* ItemData = nullptr; // e.g., Weapon class, Perk data, Door reference
	int32 BaseCost = 0;
	int32 AltCost = 0; // e.g., refill cost

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Prompt")
	FString PrimaryPrompt = "Interact";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Prompt")
	FString SecondaryPrompt; // optional


protected:

	// Return the prompt string
	virtual FString GetInteractionName(AActor* Interactor) const { return InteractionName; }


	/** Called when an actor enters range — child classes override */
	virtual void OnBeginInteract(AActor* Interactor) {}

	/** Called when an actor leaves range — child classes override */
	virtual void OnEndInteract(AActor* Interactor) {}

	// Overlap functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	// Player pressed Interact
	virtual void Interact(AActor* Interactor);

	virtual bool TryPurchase(AActor* BuyerActor);
	
	virtual FString GetInteractionPrompt(AActor* Interactor) const;

};

