// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Merc_BaseInteractable.h"
#include "Merc_Door.generated.h"

/**
 * 
 */
UCLASS()
class AMerc_Door : public AMerc_BaseInteractable
{
	GENERATED_BODY()

public:
    AMerc_Door();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    USceneComponent* DoorHinge;
    // Unique ID for this door
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    FName DoorID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    int32 DoorCost = 100; // Example cost per door

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    bool bIsOpen = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float OpenAngle = 90.f;

    UPROPERTY()
    TWeakObjectPtr<AActor> LastBuyer;

    virtual void OnBeginInteract(AActor* Interactor) override;
    virtual void OnEndInteract(AActor* Interactor) override;

public:

    virtual void Interact(AActor* Interactor) override;
    virtual bool TryPurchase(AActor* BuyerActor) override;
    virtual FString GetInteractionPrompt(AActor* Interactor) const override;
    UFUNCTION(BlueprintImplementableEvent, Category = "Door")
    void OpenDoor();
};
