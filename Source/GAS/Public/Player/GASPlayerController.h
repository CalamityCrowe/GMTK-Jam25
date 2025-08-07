// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GASPlayerController.generated.h"

//class UAbilitySystemComponent;

class UGASHUDWidget;

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
	
	UGASHUDWidget* GetHUDWidget() const { return HUDWidget; }

	virtual void LookAtCursor();

protected: 

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|HUD")
	TSubclassOf<UGASHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GAS|HUD")
	TObjectPtr<UGASHUDWidget> HUDWidget;
	

	
};
