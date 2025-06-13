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
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AMerc_WeaponDisplay::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMerc_WeaponDisplay::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMerc_WeaponDisplay::OnOverlapEnd);
	
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

	if (Player->CanAfford(WeaponCost)) 
	{
		Player->AddPoints(-WeaponCost);
		Player->AddWeaponToInventory(WeaponClass); // You create this
		return true;
	}

	return false;
}

void AMerc_WeaponDisplay::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMerc_PlayerCharacter* Player = Cast<AMerc_PlayerCharacter>(OtherActor))
	{
		bPlayerInRange = true;
		UE_LOG(LogTemp, Log, TEXT("Player entered weapon buy zone."));
		// Optionally store a reference to the player here for TryPurchase later
		Player->SetNearbyWeaponBuy(this);
		Player->ShowWeaponBuyPrompt(WeaponName.ToString(), WeaponCost);
	}
}

void AMerc_WeaponDisplay::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMerc_PlayerCharacter* Player = Cast<AMerc_PlayerCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		UE_LOG(LogTemp, Log, TEXT("Player exited weapon buy zone."));
		Player->SetNearbyWeaponBuy(nullptr);
		Player->HideWeaponBuyPrompt();
	}
}

