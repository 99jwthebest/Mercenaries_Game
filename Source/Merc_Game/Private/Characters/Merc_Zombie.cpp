// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Merc_Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AMerc_Zombie::AMerc_Zombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMerc_Zombie::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
}

// Called every frame
void AMerc_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


float AMerc_Zombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = FMath::Min(CurrentHealth, DamageAmount);
	CurrentHealth -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Zombie took %f damage. Remaining HP: %f"), DamageApplied, CurrentHealth);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return DamageApplied;
}

void AMerc_Zombie::ResetAttackCooldown()
{
	bCanAttack = true;
	UE_LOG(LogTemp, Warning, TEXT("Zombie cooldown reset. Can attack again."));
}

void AMerc_Zombie::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie died!"));

	// Optional VFX
	if (DeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
	}

	// Disable AI, collision, etc.
	DetachFromControllerPendingDestroy();
	SetActorEnableCollision(false);
	SetLifeSpan(2.0f); // Clean up actor after delay
}
