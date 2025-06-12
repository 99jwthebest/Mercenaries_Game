// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_WeaponDisplay.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Merc_PlayerCharacter.h"
#include "Merc_Gun.h"

// Sets default values
AMerc_WeaponDisplay::AMerc_WeaponDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	//TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AMerc_WeaponDisplay::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMerc_WeaponDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMerc_WeaponDisplay::TryPurchase(AMerc_PlayerCharacter* Player)
{
	if (!Player || !WeaponClass) 
		return false;

	/*if (Player->GetPoints() >= WeaponCost)
	{
		Player->AddPoints(-WeaponCost);
		Player->PickupWeapon(WeaponClass);
		return true;
	}*/

	return false;
}

