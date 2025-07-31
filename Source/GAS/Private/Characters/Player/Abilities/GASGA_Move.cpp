// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Abilities/GASGA_Move.h"
#include "Characters/Player/GASPlayerCharacter.h"
#include <Kismet/KismetMathLibrary.h>

UGASGA_Move::UGASGA_Move()
{
	AbilityID = EGASAbilityInputID::Move;
	AbilityInputID = EGASAbilityInputID::Move;
}

void UGASGA_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (AGASPlayerCharacter* Player = Cast<AGASPlayerCharacter>(ActorInfo->AvatarActor.Get())) 
	{
		if (CommitAbility(Handle, ActorInfo, ActivationInfo)) 
		{
			FVector2D Axis = Player->GetMovementAxis();
			FRotator ContRot = Player->GetControlRotation();
			const FVector XWorldVector = UKismetMathLibrary::GetRightVector(FRotator(0, ContRot.Yaw, ContRot.Roll));
			const FVector YWorldVector = UKismetMathLibrary::GetForwardVector(FRotator(0, ContRot.Yaw, 0));
			Player->AddMovementInput(XWorldVector, Axis.X, false);
			Player->AddMovementInput(YWorldVector, Axis.Y, false);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

}
