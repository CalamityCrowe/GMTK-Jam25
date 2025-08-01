// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"
#include "UI/GASHUDWidget.h"
#include "AbilitySystemComponent.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"

AGASPlayerController::AGASPlayerController()
{
}

void AGASPlayerController::CreateHUD()
{
	if (!HUDWidgetClass) 
	{
		return; 
	}
	if (HUDWidget)
	{
		return; 
	}

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (!PS) 
	{
		return;
	}

	HUDWidget = CreateWidget<UGASHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport(); 

	HUDWidget->UpdateHealthBar(	PS->GetAttributeSetBase()->GetHealthAttribute(),0,0);
}

void AGASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>(); 
	if (PS) 
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AGASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	CreateHUD(); 
}

//UAbilitySystemComponent* AGASPlayerController::GetAbilitySystemComponent() const
//{
//	return nullptr;
//}
