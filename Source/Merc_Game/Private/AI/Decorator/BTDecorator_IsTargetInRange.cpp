// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_IsTargetInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTDecorator_IsTargetInRange::UBTDecorator_IsTargetInRange()
{
	NodeName = "Is Target In Range";
}

bool UBTDecorator_IsTargetInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* OwnerActor = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!OwnerActor || !TargetActor)
		return false;

	float Distance = FVector::Dist(OwnerActor->GetActorLocation(), TargetActor->GetActorLocation());
	return Distance <= Range;
}
