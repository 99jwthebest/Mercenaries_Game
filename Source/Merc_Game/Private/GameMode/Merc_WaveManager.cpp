// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Merc_WaveManager.h"
#include "Characters/Merc_BaseEnemy.h"

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
	EnemiesToSpawn = EnemiesPerWave * CurrentWave;
	EnemiesSpawned = 0;
	AliveEnemies.Empty();
	UE_LOG(LogTemp, Error, TEXT("Current Wave: %d"), CurrentWave);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMerc_WaveManager::SpawnEnemy, TimeBetweenSpawns, true);
}

void AMerc_WaveManager::SpawnEnemy()
{
	
	if (EnemiesSpawned >= EnemiesToSpawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	FVector SpawnLocation = GetRandomSpawnPoint();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	int32 RandomIndex = FMath::RandRange(0, EnemyTypes.Num() - 1);
	TSubclassOf<AMerc_BaseEnemy> ChosenType = EnemyTypes[RandomIndex];
	AMerc_BaseEnemy* NewEnemy = GetWorld()->SpawnActor<AMerc_BaseEnemy>(ChosenType, SpawnLocation, SpawnRotation);
	if (NewEnemy)
	{
		EnemiesSpawned++;
		AliveEnemies.Add(NewEnemy);

		// Bind the delegate for when the zombie dies
		NewEnemy->OnEnemyDeath.AddDynamic(this, &AMerc_WaveManager::OnEnemyDied);
	}
	
}

void AMerc_WaveManager::OnEnemyDied(AMerc_BaseEnemy* DeadEnemy)
{
	AliveEnemies.Remove(DeadEnemy);
	CheckWaveCompletion();
}

void AMerc_WaveManager::CheckWaveCompletion()
{
	if (AliveEnemies.Num() <= 0 && EnemiesSpawned >= EnemiesToSpawn)
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

