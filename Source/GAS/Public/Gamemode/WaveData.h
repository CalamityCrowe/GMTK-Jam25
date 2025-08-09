// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/GASEnemyCharacter.h"
#include "WaveData.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FWaveModifiers
{
GENERATED_BODY()

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
TMap<TSubclassOf<AGASEnemyCharacter>, int32> Enemies;

};

USTRUCT(Blueprintable)
struct GAS_API FWaveDataRow : public FTableRowBase
{

	GENERATED_BODY()
public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWaveModifiers WaveModifiers;
};
