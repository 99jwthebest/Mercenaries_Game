// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Merc_WaveManager.generated.h"

class AMerc_BaseEnemy;

UCLASS()
class AMerc_WaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMerc_WaveManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Wave Settings")
	TArray<TSubclassOf<AMerc_BaseEnemy>> EnemyTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Wave Settings")
	int32 EnemiesPerWave = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Wave Settings")
	float TimeBetweenSpawns = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Wave Settings")
	float TimeBetweenWaves = 5.0f;

	UPROPERTY(EditInstanceOnly, Category = "Wave Settings")
	TArray<AActor*> SpawnPoints;

private:
	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveDelayTimerHandle;

	int32 CurrentWave = 0;
	int32 EnemiesSpawned = 0;
	int32 EnemiesToSpawn = 0;

	TArray<AMerc_BaseEnemy*> AliveEnemies;

	void StartWave();
	void SpawnEnemy();
	UFUNCTION()
	void OnEnemyDied(AMerc_BaseEnemy* DeadEnemy);
	void CheckWaveCompletion();
	FVector GetRandomSpawnPoint();

};
