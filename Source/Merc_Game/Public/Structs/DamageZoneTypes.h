#pragma once

#include "CoreMinimal.h"
#include "DamageZoneTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageZone
{
	GENERATED_BODY()

	UPROPERTY()
	class UCapsuleComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.0f;
};