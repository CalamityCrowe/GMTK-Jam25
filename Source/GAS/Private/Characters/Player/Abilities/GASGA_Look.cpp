// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Abilities/GASGA_Look.h"
#include "Characters/Player/GASPlayerCharacter.h"
#include <Kismet/KismetMathLibrary.h>
UGASGA_Look::UGASGA_Look()
{
	AbilityID = EGASAbilityInputID::Look; 
	AbilityInputID = EGASAbilityInputID::Look;
}

void UGASGA_Look::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (AGASPlayerCharacter* Character = Cast<AGASPlayerCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			FVector2D Axis = Character->GetLookAxis();
			FRotator ContRot = Character->GetControlRotation();
			if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
			{
				PC->AddYawInput(Axis.X);
				PC->AddPitchInput(Axis.Y);
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
