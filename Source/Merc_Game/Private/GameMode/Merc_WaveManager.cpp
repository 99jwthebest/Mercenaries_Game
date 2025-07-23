// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Merc_WaveManager.h"

// Sets default values
AMerc_WaveManager::AMerc_WaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMerc_WaveManager::BeginPlay()
{
	Super::BeginPlay();
	StartWave();
}

// Called every frame
void AMerc_WaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMerc_WaveManager::StartWave()
{
	CurrentWave++;
	ZombiesToSpawn = ZombiesPerWave * CurrentWave;
	ZombiesSpawned = 0;
	AliveZombies.Empty();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMerc_WaveManager::SpawnZombie, TimeBetweenSpawns, true);
}

void AMerc_WaveManager::SpawnZombie()
{
	
	if (ZombiesSpawned >= ZombiesToSpawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	FVector SpawnLocation = GetRandomSpawnPoint();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AMerc_Zombie* NewZombie = GetWorld()->SpawnActor<AMerc_Zombie>(ZombieClass, SpawnLocation, SpawnRotation);
	if (NewZombie)
	{
		ZombiesSpawned++;
		AliveZombies.Add(NewZombie);

		// Bind the delegate for when the zombie dies
		//NewZombie->OnDeathDelegate.AddUObject(this, &AMerc_WaveManager::OnZombieDied);
	}
	
}

void AMerc_WaveManager::OnZombieDied(AMerc_Zombie* DeadZombie)
{
	AliveZombies.Remove(DeadZombie);
	CheckWaveCompletion();
}

void AMerc_WaveManager::CheckWaveCompletion()
{
	if (AliveZombies.Num() <= 0 && ZombiesSpawned >= ZombiesToSpawn)
	{
		GetWorld()->GetTimerManager().SetTimer(WaveDelayTimerHandle, this, &AMerc_WaveManager::StartWave, TimeBetweenWaves, false);
	}
}

FVector AMerc_WaveManager::GetRandomSpawnPoint()
{
	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn points set!"));
		return FVector::ZeroVector;
	}

	int32 Index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	return SpawnPoints[Index]->GetActorLocation();
}

