// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"
#include "UI/GASHUDWidget.h"
#include "AbilitySystemComponent.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"
#include <Kismet/KismetMathLibrary.h>

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

	HUDWidget->UpdateHealthBar(PS->GetAttributeSetBase()->GetHealthAttribute(), 0, 0);
	HUDWidget->UpdateLoop();
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

void AGASPlayerController::LookAtCursor()
{
	FHitResult Hit;

	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		FVector LookAtLocation = Hit.ImpactPoint;
		FRotator LookAtRotation;
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), LookAtLocation);
		FRotator CurrentRotation = GetPawn()->GetActorRotation();
		LookAtRotation = FRotator(CurrentRotation.Pitch, LookAtRotation.Yaw, CurrentRotation.Roll);
		SetControlRotation(LookAtRotation);
	}

}

//UAbilitySystemComponent* AGASPlayerController::GetAbilitySystemComponent() const
//{
//	return nullptr;
//}
