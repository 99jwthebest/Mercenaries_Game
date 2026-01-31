#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHitDamageable.generated.h"


UENUM(BlueprintType)
enum class EHitKind : uint8
{
    Bullet     UMETA(DisplayName = "Bullet"),
    Melee      UMETA(DisplayName = "Melee"),
    Explosion  UMETA(DisplayName = "Explosion"),
    Other      UMETA(DisplayName = "Other"),
};

USTRUCT(BlueprintType)
struct FHitSpec
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EHitKind HitKind = EHitKind::Other;

	// Who caused it (player, explosive actor, etc.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* InstigatorActor = nullptr;

	// Which component on the victim was hit (head/limb capsule, body, etc.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPrimitiveComponent* HitComponent = nullptr;

	// Raw damage before multiplier
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BaseDamage = 0.f;

	// Knockback parameters (optional, but great for RE4 style)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ImpulseDir = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ImpulseStrength = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UpwardBoost = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StunTime = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bFaceInstigator = false;
};


UINTERFACE(MinimalAPI) //, BlueprintType
class UHitDamageable : public UInterface
{
    GENERATED_BODY()
};

class IHitDamageable
{
    GENERATED_BODY()

public:
    // This will return the damage multiplier based on the hit component
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    float GetDamageMultiplierFromComponent(UPrimitiveComponent* HitComponent) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void ApplyHit(const FHitSpec& Spec);
};