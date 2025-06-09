// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Merc_PlayerHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class UMerc_PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	// Public functions to update UI
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int32 Current, int32 Max);

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float Current, float Max);

	UFUNCTION(BlueprintCallable)
	void UpdateGrenades(int32 Count);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrenadeText;

};
