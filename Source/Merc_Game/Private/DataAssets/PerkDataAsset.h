// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PerkDataAsset.generated.h"

UCLASS(BlueprintType)
class UPerkDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
    // Unique ID for this perk
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perk")
    FName PerkID;

    // Name displayed to the player
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perk")
    FString DisplayName;

    // Description shown in UI
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perk")
    FString Description;

    // Cost of the perk
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perk")
    int32 Cost = 1000;
};
