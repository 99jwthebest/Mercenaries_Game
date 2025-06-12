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

public:
	FORCEINLINE FName GetWeaponName() const { return WeaponName; }
	FORCEINLINE int32 GetWeaponCost() const { return WeaponCost; }
	FORCEINLINE UTexture2D* GetWeaponIcon() const { return WeaponIcon; }
};
