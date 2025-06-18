// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTargetInRange.generated.h"

/**
 * 
 */
UCLASS()
class UBTDecorator_IsTargetInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsTargetInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Condition")
	float Range = 100.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

};
