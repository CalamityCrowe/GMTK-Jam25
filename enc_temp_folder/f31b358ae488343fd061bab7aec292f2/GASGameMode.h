// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gamemode/WaveData.h"
#include "GASGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AGASGameMode : public AGameModeBase
{
	GENERATED_BODY()
public: 
	AGASGameMode();

	int32 GetEnemiesKilled() const { return EnemiesKilled; }
	float GetTimeSurvived() const { return TimeSurvived; }

	void IncrementEnemiesKilled() { EnemiesKilled++; }



protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|WaveData")
	void SpawnWave();

private: 
	UPROPERTY(EditDefaultsOnly, Category = "GAS|WaveData", meta = (AllowPrivateAccess = true))
	UDataTable* WaveDataTable;

	int32 CurrentWaveIndex = 0;

	void StartNextWave();


	int32 EnemiesKilled = 0;

	float TimeSurvived = 0.0f;
};
