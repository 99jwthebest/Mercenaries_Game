// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatTrackerComponent.h"

// Sets default values for this component's properties
UStatTrackerComponent::UStatTrackerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Score = 0;
	Kills = 0;
	Headshots = 0;
	Combo = 0;
	TimeSurvived = 0.f;

}

void UStatTrackerComponent::AddScore(int32 Amount)
{
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score = %d"), Score);
	OnScoreChanged.Broadcast(Score);
}

void UStatTrackerComponent::AddKill()
{
	Kills++;

	UE_LOG(LogTemp, Warning, TEXT("Kills = %d"), Kills);
	//if (bWasHeadshot)
	//{
	//	Headshots++;
	//	AddScore(100); // Bonus for headshot
	//}
	//else
	//{
	//	AddScore(50); // Normal kill
	//}
}

void UStatTrackerComponent::ResetCombo()
{
	Combo = 0;
}

void UStatTrackerComponent::AddCombo(int32 Amount)
{
	Combo += Amount;
}

void UStatTrackerComponent::UpdateSurvivalTime(float DeltaSeconds)
{
	TimeSurvived += DeltaSeconds;
}



