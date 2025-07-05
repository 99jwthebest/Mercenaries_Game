#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHitDamageable.generated.h"

UINTERFACE(MinimalAPI)
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
};