// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Merc_PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

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

void UMerc_PlayerHUDWidget::SetHealthPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UMerc_PlayerHUDWidget::UpdateGrenades(int32 Count)
{
	if (GrenadeText)
	{
		GrenadeText->SetText(FText::FromString(FString::Printf(TEXT("Grenades: %d"), Count)));
	}
}

void UMerc_PlayerHUDWidget::SetWeaponIcon(UTexture2D* NewIcon)
{
	if (WeaponIcon)
	{
		WeaponIcon->SetBrushFromTexture(NewIcon);
	}
}

void UMerc_PlayerHUDWidget::UpdateScore(int32 Count)
{
	if (PlayerScoreText)
	{
		PlayerScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Count)));
	}
}

void UMerc_PlayerHUDWidget::UpdateMoney(int32 Count)
{
	if (PlayerMoneyText)
	{
		PlayerMoneyText->SetText(FText::FromString(FString::Printf(TEXT("Money $ %d"), Count)));
	}
}
