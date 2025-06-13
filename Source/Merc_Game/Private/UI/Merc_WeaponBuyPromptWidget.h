// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Merc_WeaponBuyPromptWidget.generated.h"

class UTextBlock;
//class UImage;

UCLASS()
class UMerc_WeaponBuyPromptWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPromptText(const FString& Text);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PromptText;

};
