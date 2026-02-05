// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Merc_BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatTrackerComponent.h"


// Sets default values
AMerc_BaseEnemy::AMerc_BaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMerc_BaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

// Called every frame
void AMerc_BaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float AMerc_BaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Old simple implementation before ApplyHit, just in case
	//const float DamageApplied = FMath::Min(CurrentHealth, DamageAmount);
	//CurrentHealth -= DamageApplied;

	//UE_LOG(LogTemp, Warning, TEXT("Zombie took %f damage. Remaining HP: %f"), DamageApplied, CurrentHealth);

	//// Save the instigator for later (used in Die)
	//LastInstigator = EventInstigator;

	//if (CurrentHealth <= 0.f)
	//{
	//	Die();
	//}

	//return DamageApplied;

	FHitSpec Spec;
	Spec.HitKind = EHitKind::Other;
	Spec.BaseDamage = DamageAmount;
	Spec.InstigatorActor = EventInstigator ? EventInstigator->GetPawn() : nullptr;
	Spec.HitComponent = nullptr; // TakeDamage doesn't tell us which component

	IHitDamageable::Execute_ApplyHit(this, Spec);
	return DamageAmount;
}

void AMerc_BaseEnemy::ResetAttackCooldown()
{
	bCanAttack = true;
	UE_LOG(LogTemp, Warning, TEXT("Zombie cooldown reset. Can attack again."));
}

void AMerc_BaseEnemy::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("Zombie died!"));
	NotifyEnemyDead();

	// Reward score to killer
	//if (LastInstigator && LastInstigator->GetPawn())
	//{
	//	AActor* Killer = LastInstigator->GetPawn(); // Could be player or bot

	//	if (UStatTrackerComponent* StatTracker = Killer->FindComponentByClass<UStatTrackerComponent>())
	//	{
	//		// Example: +200 for kill
	//		StatTracker->AddScore(200);
	//		StatTracker->AddMoney(200);
	//		StatTracker->AddKill();  // You can create AddKill() method too
	//	}
	//}  **** Probably delete later!!!!! *******

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

float AMerc_BaseEnemy::GetDamageMultiplierFromComponent_Implementation(UPrimitiveComponent* HitComponent) const
{
	for (const FDamageZone& Zone : DamageZones)
	{
		if (Zone.Collider == HitComponent)
		{
			return Zone.DamageMultiplier;
		}
	}
	return 1.0f;  // Default no-multiplier
}

void AMerc_BaseEnemy::ApplyHit_Implementation(const FHitSpec& Spec)
{
	if (bIsDead) return;

	// 1) Determine controller instigator for score-credit (works with your existing Die() logic)
	AController* InstigatorController = nullptr;

	if (APawn* InstPawn = Cast<APawn>(Spec.InstigatorActor))
	{
		InstigatorController = InstPawn->GetController();
	}
	else if (AActor* InstActor = Spec.InstigatorActor)
	{
		// If later you pass a projectile or weapon actor, you can optionally pull controller differently.
		// For now, leave nullptr if not a pawn.
	}

	// Keep your existing field updated so Die() can reward the killer
	LastInstigator = InstigatorController;

	// 2) Compute final damage using your existing multiplier helper
	const float Multiplier = GetDamageMultiplierFromComponent_Implementation(Spec.HitComponent);
	const float FinalDamage = FMath::Max(0.f, Spec.BaseDamage * Multiplier);

	if (FinalDamage <= 0.f) return;

	// 3) Apply health (authoritative truth lives here now)
	const float DamageApplied = FMath::Min(CurrentHealth, FinalDamage);
	CurrentHealth -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Enemy took %f damage (Base=%f, Mult=%f). Remaining HP: %f"),
		DamageApplied, Spec.BaseDamage, Multiplier, CurrentHealth);

	// 4) If dead, die (and don't do reaction unless you want death reactions)
	if (CurrentHealth <= 0.f)
	{
		// Award score BEFORE Die() destroys controller/collision and such
		AwardOnKill(Spec, Multiplier);

		Die();
		return;
	}

	// Optional: award hit score here (if you want per-hit points)
	// AwardOnHit(Spec, Multiplier);

	// 5) Non-lethal reaction hooks
	HandleHitReaction(Spec);
}

void AMerc_BaseEnemy::NotifyEnemyDead()
{
	OnEnemyDeath.Broadcast(this);
}

void AMerc_BaseEnemy::InitCapsuleColliders()
{
	// Example implementation — override in child class
/*
	UE_LOG(LogTemp, Warning, TEXT("InitCapsuleColliders called"));

	HeadCollider = CreateZoneCollider(TEXT("HeadCollider"), TEXT("head"), FVector(10.f, 15.f, 0), 2.0f);
	BodyCollider = CreateZoneCollider(TEXT("BodyCollider"), TEXT("Spine2"), FVector(20.f, 30.f, 0), 1.0f);

	UE_LOG(LogTemp, Warning, TEXT("Final DamageZones.Num = %d"), DamageZones.Num());
*/
}

UCapsuleComponent* AMerc_BaseEnemy::CreateZoneCollider(FName Name, FName Bone, FVector Size, float Multiplier)
{
	UCapsuleComponent* Capsule = CreateDefaultSubobject<UCapsuleComponent>(Name);
	Capsule->SetupAttachment(GetMesh(), Bone);
	Capsule->InitCapsuleSize(Size.X, Size.Y);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	FDamageZone Zone;
	Zone.Collider = Capsule;
	Zone.DamageMultiplier = Multiplier;
	//DamageZones.Add(Zone);

	return Capsule;
}

void AMerc_BaseEnemy::AddingDamageZones()
{
	// Example implementation — override in child class
/*
	DamageZones.Empty();
	DamageZones.Add({ HeadCollider, HeadDamageMultiplier });
	DamageZones.Add({ BodyCollider, BodyDamageMultiplier });

	UE_LOG(LogTemp, Warning, TEXT("Final DamageZones.Num = %d"), DamageZones.Num());

	for (const FDamageZone& Zone : DamageZones)
	{
		if (Zone.Collider)
		{
			UE_LOG(LogTemp, Warning, TEXT("Zone Collider: %s | %p"), *Zone.Collider->GetName(), Zone.Collider);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("A DamageZone was added with nullptr collider!"));
		}
	}
*/
}

void AMerc_BaseEnemy::HandleHitReaction(const FHitSpec& Spec)
{
	// Optionally rotate to face the instigator
	FaceInstigatorIfNeeded(Spec);

	// Apply knockback only if this enemy opts in
	ApplyKnockbackIfAllowed(Spec);

	// Apply stun (base supports it; child can opt out if needed)
	if (Spec.StunTime > 0.f)
	{
		ApplyStunIfAllowed(Spec.StunTime);
	}

	// Optional: play hit react montages per HitKind in derived classes
	// Base class does nothing here so you don't force every enemy to have animations.
}

void AMerc_BaseEnemy::FaceInstigatorIfNeeded(const FHitSpec& Spec)
{
	if (!Spec.bFaceInstigator || !Spec.InstigatorActor) return;

	const FVector ToInst = (Spec.InstigatorActor->GetActorLocation() - GetActorLocation());
	if (ToInst.IsNearlyZero()) return;

	const FRotator NewRot = FRotationMatrix::MakeFromX(ToInst).Rotator();
	SetActorRotation(FRotator(0.f, NewRot.Yaw, 0.f));
}

void AMerc_BaseEnemy::ApplyKnockbackIfAllowed(const FHitSpec& Spec)
{
	if (!CanBeKnockedBack()) return;

	if (Spec.ImpulseStrength <= 0.f) return;
	if (Spec.ImpulseDir.IsNearlyZero()) return;

	const FVector Dir = Spec.ImpulseDir.GetSafeNormal();
	const FVector LaunchVel = (Dir * Spec.ImpulseStrength) + (FVector::UpVector * Spec.UpwardBoost);

	LaunchCharacter(LaunchVel, true, true);
}

void AMerc_BaseEnemy::ApplyStunIfAllowed(float Duration)
{
	if (!CanBeStunned()) return;
	if (Duration <= 0.f) return;

	bIsStunned = true;

	// Simple, engine-agnostic “stun”: stop movement for now.
	// If you use AIController logic/BT, we can stop brain logic here too.
	GetCharacterMovement()->StopMovementImmediately();

	// If you have an AIController and want to freeze it:
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->StopMovement();
		// Optional later:
		// if (UBrainComponent* Brain = AIC->GetBrainComponent()) Brain->StopLogic(TEXT("Stunned"));
	}

	GetWorldTimerManager().ClearTimer(StunTimer);
	GetWorldTimerManager().SetTimer(StunTimer, this, &AMerc_BaseEnemy::ClearStun, Duration, false);
}

void AMerc_BaseEnemy::ClearStun()
{
	bIsStunned = false;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		// Optional later:
		// if (UBrainComponent* Brain = AIC->GetBrainComponent()) Brain->RestartLogic();
	}
}

UStatTrackerComponent* AMerc_BaseEnemy::GetInstigatorStatTracker(const FHitSpec& Spec) const
{
	APawn* InstPawn = Cast<APawn>(Spec.InstigatorActor);
	if (!InstPawn) return nullptr;

	return InstPawn->FindComponentByClass<UStatTrackerComponent>();
}

void AMerc_BaseEnemy::AwardOnHit(const FHitSpec& Spec, float Multiplier)
{

}

void AMerc_BaseEnemy::AwardOnKill(const FHitSpec& Spec, float Multiplier)
{
	UStatTrackerComponent* StatTracker = GetInstigatorStatTracker(Spec);
	if (!StatTracker) return;

	// Decide headshot based on multiplier (simple + consistent)
	const bool bIsHeadshot = (Multiplier >= 2.0f);  // ****** probably want to make it a variable ******///

	// You control these values
	const int32 KillScore = 200;  // ****** make variables !!!!!!!
	const int32 KillMoney = 200;

	StatTracker->AddKill();
	StatTracker->AddScore(KillScore);
	StatTracker->AddMoney(KillMoney);

	// ******** Probably will keep these bonuses just adjust values and make variables lol!!!!
	if (bIsHeadshot)
	{
		StatTracker->AddHeadshot();

		// bonus score/money if you want
		const int32 HeadshotBonus = 100;
		StatTracker->AddScore(HeadshotBonus);
		StatTracker->AddMoney(HeadshotBonus);
	}

	// Optional: combo increments on kill (or do this on hit)
	StatTracker->AddCombo(1);
}

