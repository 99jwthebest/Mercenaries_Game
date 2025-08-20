#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBuyer.generated.h"

UINTERFACE(MinimalAPI)
class UBuyer : public UInterface
{
	GENERATED_BODY()
};

class IBuyer
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buying")
    bool TryPurchase(EItemType ItemType, UObject* ItemData, int32 Cost);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buying")
    bool HasItem(EItemType ItemType, UObject* ItemData) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buying")
    void ShowBuyPrompt(AMerc_BaseInteractable* Interactable);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buying")
    void HideBuyPrompt();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Buying")
    void SetNearbyInteractable(class AMerc_BaseInteractable* NewInteractable);
};