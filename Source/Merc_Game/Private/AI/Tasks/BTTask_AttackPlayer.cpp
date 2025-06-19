// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_AttackPlayer.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Merc_Zombie.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World)
		return EBTNodeResult::Failed;

	APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!AIPawn)
		return EBTNodeResult::Failed;

	AMerc_Zombie* Zombie = Cast<AMerc_Zombie>(AIPawn);
	if (!Zombie) 
		return EBTNodeResult::Failed;

	if (!Zombie->bCanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie cannot attack: cooldown."));
		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Warning, TEXT("Zombie attacks player (task)."));

	// --- Sphere Trace ---
	const float TraceDistance = 150.f;
	const float TraceRadius = 60.f;

	const FVector Start = Zombie->GetActorLocation() + FVector(0, 0, 50);
	const FVector End = Start + Zombie->GetActorForwardVector() * TraceDistance;

	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Zombie);

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		World,
		Start,
		End,
		TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.0f
	);

	if (bHit)
	{
		TSet<AActor*> DamagedActors;

		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !DamagedActors.Contains(HitActor))
			{
				DamagedActors.Add(HitActor);

				UGameplayStatics::ApplyDamage(HitActor, Zombie->Damage, Zombie->GetController(), Zombie, nullptr);
				UE_LOG(LogTemp, Log, TEXT("Zombie melee hit: %s for %f damage"), *HitActor->GetName(), Zombie->Damage);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie melee trace missed."));
	}

	// --- Cooldown ---
	Zombie->bCanAttack = false;
	World->GetTimerManager().SetTimer(Zombie->AttackCooldownTimer, Zombie, &AMerc_Zombie::ResetAttackCooldown, Zombie->AttackCooldown, false);

	return EBTNodeResult::Succeeded;
}

