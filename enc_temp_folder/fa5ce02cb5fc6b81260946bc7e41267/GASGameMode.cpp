// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameMode.h"

AGASGameMode::AGASGameMode()
{

}

void AGASGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AGASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeSurvived += DeltaSeconds;
}


void AGASGameMode::StartNextWave()
{
	if (WaveDataTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveDataTable is not set!"));
		return;
	}

	TArray<FName> RowNames = WaveDataTable->GetRowNames();
	if (CurrentWaveIndex >= RowNames.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("No more waves available!"));
		return;
	}

	const FWaveDataRow* WaveData = WaveDataTable->FindRow<FWaveDataRow>(RowNames[CurrentWaveIndex], TEXT("StartNextWave"));
	if (WaveData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave data not found for index %d!"), CurrentWaveIndex);
		return;
	}


	SpawnWave();
	CurrentWaveIndex++;
}
