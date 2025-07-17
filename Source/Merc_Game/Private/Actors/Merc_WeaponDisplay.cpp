// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Merc_WeaponDisplay.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Merc_Gun.h"
#include "Interfaces/IWeaponBuyer.h"


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

bool AMerc_WeaponDisplay::TryPurchase(AActor* BuyerActor)
{
	if (!BuyerActor || !WeaponClass || !BuyerActor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass()))
		return false;

	if (IWeaponBuyer::Execute_HasWeapon(BuyerActor, WeaponClass))
	{
		AMerc_Gun* OwnedGun = IWeaponBuyer::Execute_GetWeaponByClass(BuyerActor, WeaponClass);
		if (OwnedGun && OwnedGun->CanRefillAmmo())
		{
			if (IWeaponBuyer::Execute_TryRefillAmmo(BuyerActor, WeaponClass, AmmoCost))
			{
				LastBuyer = BuyerActor;
				return true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ammo is already full. Cannot refill."));
		}

		return false;
	}

	// Otherwise, it's a new weapon purchase
	if (IWeaponBuyer::Execute_TryBuyWeapon(BuyerActor, WeaponClass, WeaponCost))
	{
		LastBuyer = BuyerActor;
		return true;
	}

	return false;
}


void AMerc_WeaponDisplay::ClearBuyerIfNoLongerOwned(AActor* BuyerActor)
{
	if (LastBuyer == BuyerActor && !IWeaponBuyer::Execute_HasWeapon(BuyerActor, WeaponClass))
	{
		LastBuyer = nullptr;
	}
}


void AMerc_WeaponDisplay::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass()))
	{
		bPlayerInRange = true;
		LastBuyer = OtherActor;

		IWeaponBuyer::Execute_SetNearbyWeaponBuy(OtherActor, this);

		const bool bPlayerOwnsWeapon = IWeaponBuyer::Execute_HasWeapon(OtherActor, WeaponClass);

		if (bPlayerOwnsWeapon)
		{
			IWeaponBuyer::Execute_ShowWeaponBuyPrompt(OtherActor, WeaponName.ToString(), AmmoCost, true);
		}
		else
		{
			IWeaponBuyer::Execute_ShowWeaponBuyPrompt(OtherActor, WeaponName.ToString(), WeaponCost, false);
		}

		UE_LOG(LogTemp, Log, TEXT("Player entered weapon zone. Owns weapon? %s"), bPlayerOwnsWeapon ? TEXT("Yes") : TEXT("No"));
	}

}

void AMerc_WeaponDisplay::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UWeaponBuyer::StaticClass()))
	{
		bPlayerInRange = false;
		UE_LOG(LogTemp, Log, TEXT("Player exited weapon buy zone."));
		IWeaponBuyer::Execute_SetNearbyWeaponBuy(OtherActor, nullptr);
		IWeaponBuyer::Execute_HideWeaponBuyPrompt(OtherActor);
	}
}

