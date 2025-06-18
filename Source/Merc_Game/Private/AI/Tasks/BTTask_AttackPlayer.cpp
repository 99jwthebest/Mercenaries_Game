// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_AttackPlayer.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// We'll improve this later
	UE_LOG(LogTemp, Warning, TEXT("AI Attacking Player!"));
	return EBTNodeResult::Succeeded;
}