// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatTrackerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoneyAmount);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UStatTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatTrackerComponent();

private:

	// Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Score;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Money;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Kills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Headshots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Combo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TimeSurvived;
	

public:

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = "true"))
	FOnScoreChanged OnScoreChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = "true"))
	FOnMoneyChanged OnMoneyChanged;

	void AddScore(int32 Amount);
	void AddMoney(int32 Amount);
	void AddKill();
	void ResetCombo();
	void AddCombo(int32 Amount);
	void UpdateSurvivalTime(float DeltaSeconds);



	int32 GetScore() const { return Score; }
	int32 GetMoney() const { return Money; }

		
};
