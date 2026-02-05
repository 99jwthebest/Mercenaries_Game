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

    // ---------- Factory helpers ----------
    static FHitSpec MakeBullet(AActor* Instigator, UPrimitiveComponent* HitComp, float InBaseDamage, const FVector& ShotDir)
    {
        FHitSpec Spec;
        Spec.HitKind = EHitKind::Bullet;
        Spec.InstigatorActor = Instigator;
        Spec.HitComponent = HitComp;
        Spec.BaseDamage = InBaseDamage;

        // Bullets default to no knockback/stun
        // Keep direction if you later want directional flinch or decals.
        Spec.ImpulseDir = ShotDir;

        Spec.ImpulseStrength = 0.f;
        Spec.UpwardBoost = 0.f;
        Spec.StunTime = 0.f;
        Spec.bFaceInstigator = false;
        return Spec;
    }

    static FHitSpec MakeMelee(AActor* Instigator, UPrimitiveComponent* HitComp, float InBaseDamage, const FVector& Dir,
        float KnockbackStrength, float InUpwardBoost, float InStunTime, bool bInFaceInstigator = true)
    {
        FHitSpec Spec;
        Spec.HitKind = EHitKind::Melee;
        Spec.InstigatorActor = Instigator;
        Spec.HitComponent = HitComp;
        Spec.BaseDamage = InBaseDamage;

        Spec.ImpulseDir = Dir;
        Spec.ImpulseStrength = KnockbackStrength;
        Spec.UpwardBoost = InUpwardBoost;
        Spec.StunTime = InStunTime;
        Spec.bFaceInstigator = bInFaceInstigator;
        return Spec;
    }

    static FHitSpec MakeExplosion(AActor* Instigator, UPrimitiveComponent* HitComp, float InBaseDamage, const FVector& Dir,
        float KnockbackStrength, float InUpwardBoost, float InStunTime, bool bInFaceInstigator = false)
    {
        FHitSpec Spec;
        Spec.HitKind = EHitKind::Explosion;
        Spec.InstigatorActor = Instigator;
        Spec.HitComponent = HitComp;
        Spec.BaseDamage = InBaseDamage;

        Spec.ImpulseDir = Dir;
        Spec.ImpulseStrength = KnockbackStrength;
        Spec.UpwardBoost = InUpwardBoost;
        Spec.StunTime = InStunTime;
        Spec.bFaceInstigator = bInFaceInstigator;
        return Spec;
    }
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