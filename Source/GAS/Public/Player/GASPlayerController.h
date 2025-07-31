// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GASPlayerController.generated.h"

//class UAbilitySystemComponent;


/**
 * 
 */
UCLASS()
class GAS_API AGASPlayerController : public APlayerController
{
	GENERATED_BODY()
public: 
	AGASPlayerController();

	void CreateHUD();
	
protected: 

	virtual void OnPossess(APawn* InPawn) override;



	
	
};
