// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatTrackerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UStatTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatTrackerComponent();

	// Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Kills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Headshots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Combo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimeSurvived;

	// Public Functions
	void AddScore(int32 Amount);
	void AddKill();
	void ResetCombo();
	void AddCombo(int32 Amount);
	void UpdateSurvivalTime(float DeltaSeconds);


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	int32 GetScore() const { return Score; }
		
};
