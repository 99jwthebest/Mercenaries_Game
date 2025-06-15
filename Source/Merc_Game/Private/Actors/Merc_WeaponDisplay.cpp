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

	// Check if the player already owns this weapon
	if (Player->HasWeapon(WeaponClass))
	{
		AMerc_Gun* OwnedGun = Player->GetWeaponByClass(WeaponClass);
		if (OwnedGun && OwnedGun->CanRefillAmmo())
		{
			if (Player->GetPoints() >= AmmoCost)
			{
				Player->AddPoints(-AmmoCost);
				OwnedGun->Refill();
				return true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ammo is already full. Cannot refill."));
		}

		return false; // Don’t allow refill if already full
	}

	// Otherwise, it's a new weapon purchase
	if (Player->GetPoints() >= WeaponCost)
	{
		Player->AddPoints(-WeaponCost);
		Player->AddWeaponToInventory(WeaponClass, true);
		return true;
	}

	return false;
}

void AMerc_WeaponDisplay::ClearBuyerIfNoLongerOwned(AMerc_PlayerCharacter* Player)
{
	if (LastBuyer == Player && !Player->HasWeapon(WeaponClass))
	{
		LastBuyer = nullptr;
	}
}

void AMerc_WeaponDisplay::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMerc_PlayerCharacter* Player = Cast<AMerc_PlayerCharacter>(OtherActor))
	{
		bPlayerInRange = true;
		Player->SetNearbyWeaponBuy(this);

		const bool bPlayerOwnsWeapon = Player->HasWeapon(WeaponClass); // You'll need to have this function

		if (bPlayerOwnsWeapon)
		{
			Player->ShowWeaponBuyPrompt(WeaponName.ToString(), AmmoCost, true); // true = isRefill
		}
		else
		{
			Player->ShowWeaponBuyPrompt(WeaponName.ToString(), WeaponCost, false);
		}

		UE_LOG(LogTemp, Log, TEXT("Player entered weapon zone. Owns weapon? %s"), bPlayerOwnsWeapon ? TEXT("Yes") : TEXT("No"));
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

