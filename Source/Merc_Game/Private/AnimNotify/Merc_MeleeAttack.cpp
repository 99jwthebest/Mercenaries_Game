// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Merc_MeleeAttack.h"
#include "GameFramework/Character.h"
#include "Components/Merc_MeleeComponent.h"

void UMerc_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

    UE_LOG(LogTemp, Warning, TEXT("triggering notify"))
        if (!MeshComp || !MeshComp->GetWorld()) // Ensure MeshComp and World are valid
            return;

    const AActor* OwnerActor = MeshComp->GetOwner();

    if (!OwnerActor || !OwnerActor->GetWorld()->IsGameWorld()) // Ensure it's a valid actor in a game world
        return;

    PlayerCharacter = Cast<ACharacter>(MeshComp->GetOwner());

    if (PlayerCharacter)
        MeleeComp = OwnerActor->FindComponentByClass<UMerc_MeleeComponent>();

    if (!MeleeComp)
        return;

    //if (!CharacterMoveComp->IsRaging())
    MeleeComp->MeleeTrace();
            //DrawWeaponCollision(End, Radius, AmountOfDamage, DamageTypeClass);
}
