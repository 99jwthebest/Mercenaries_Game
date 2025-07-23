// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Merc_WaveManager.generated.h"

UCLASS()
class AMerc_WaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMerc_WaveManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	TSubclassOf<class AMerc_Zombie> ZombieClass;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	int32 ZombiesPerWave = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float TimeBetweenSpawns = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float TimeBetweenWaves = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<AActor*> SpawnPoints;

private:
	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveDelayTimerHandle;

	int32 CurrentWave = 0;
	int32 ZombiesSpawned = 0;
	int32 ZombiesToSpawn = 0;

	TArray<AMerc_Zombie*> AliveZombies;

	void StartWave();
	void SpawnZombie();
	void OnZombieDied(AMerc_Zombie* DeadZombie);
	void CheckWaveCompletion();
	FVector GetRandomSpawnPoint();

};
