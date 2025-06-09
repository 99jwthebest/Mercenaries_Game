// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Merc_PlayerHUDWidget.h"
#include "Components/TextBlock.h"

void UMerc_PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Optionally initialize with placeholder text
	UpdateAmmo(0, 0);
	//UpdateHealth(100.f, 100.f);
	//UpdateGrenades(0);
}

void UMerc_PlayerHUDWidget::UpdateAmmo(int32 Current, int32 Max)
{
	if (AmmoText)
	{
		AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Current, Max)));
	}
}

void UMerc_PlayerHUDWidget::UpdateHealth(float Current, float Max)
{
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.0f / %.0f"), Current, Max)));
	}
}

void UMerc_PlayerHUDWidget::UpdateGrenades(int32 Count)
{
	if (GrenadeText)
	{
		GrenadeText->SetText(FText::FromString(FString::Printf(TEXT("Grenades: %d"), Count)));
	}
}
