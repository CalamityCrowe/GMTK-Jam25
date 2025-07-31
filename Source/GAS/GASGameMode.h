// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

protected:

	virtual void BeginPlay() override;
};
