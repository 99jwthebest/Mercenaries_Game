#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponBuyer.generated.h"

UINTERFACE(MinimalAPI)
class UWeaponBuyer : public UInterface
{
	GENERATED_BODY()
};

class IWeaponBuyer
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	bool TryBuyWeapon(TSubclassOf<class AMerc_Gun> WeaponClass, int32 Cost);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	bool TryRefillAmmo(TSubclassOf<class AMerc_Gun> WeaponClass, int32 Cost);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	bool HasWeapon(TSubclassOf<class AMerc_Gun> WeaponClass) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	class AMerc_Gun* GetWeaponByClass(TSubclassOf<class AMerc_Gun> WeaponClass);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void ShowWeaponBuyPrompt(const FString& WeaponName, int32 Cost, bool bIsRefill);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void HideWeaponBuyPrompt();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void SetNearbyInteractable(class AMerc_BaseInteractable* NewInteractable);
};