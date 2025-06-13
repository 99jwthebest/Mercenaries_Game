// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Merc_WeaponBuyPromptWidget.h"
#include "Components/TextBlock.h"
//#include "Components/Image.h"

void UMerc_WeaponBuyPromptWidget::SetPromptText(const FString& Text)
{
	if (PromptText)
	{
		PromptText->SetText(FText::FromString(Text));
	}
}
