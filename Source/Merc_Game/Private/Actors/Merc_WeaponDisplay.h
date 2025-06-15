// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Merc_WeaponDisplay.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AMerc_Gun;

UCLASS()
class AMerc_WeaponDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMerc_WeaponDisplay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called by player when they try to buy this weapon
	bool TryPurchase(class AMerc_PlayerCharacter* Player);

	void ClearBuyerIfNoLongerOwned(AMerc_PlayerCharacter* Player);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AMerc_Gun> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 WeaponCost = 500;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName WeaponName = "Pistol"; // Display name for UI

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UTexture2D* WeaponIcon; // Optional: for HUD or interaction prompt

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 AmmoCost = 200;

	bool bPlayerInRange = false;

	TWeakObjectPtr<AMerc_PlayerCharacter> LastBuyer;

public:
	FORCEINLINE TSubclassOf<AMerc_Gun> GetWeaponClass() const { return WeaponClass; }
	FORCEINLINE FName GetWeaponName() const { return WeaponName; }
	FORCEINLINE int32 GetWeaponCost() const { return WeaponCost; }
	FORCEINLINE UTexture2D* GetWeaponIcon() const { return WeaponIcon; }
	FORCEINLINE int32 GetWeaponAmmoCost() const { return AmmoCost; }

};
