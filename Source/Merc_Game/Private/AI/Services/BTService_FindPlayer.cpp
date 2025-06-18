// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_FindPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"


UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(ControlledPawn->GetWorld(), 0);

	if (PlayerActor)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerActor);
	}
}
