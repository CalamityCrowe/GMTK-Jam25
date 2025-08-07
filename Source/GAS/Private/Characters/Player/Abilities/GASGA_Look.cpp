// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Abilities/GASGA_Look.h"
#include "Characters/Player/GASPlayerCharacter.h"
#include "Player/GASPlayerController.h"
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
			AGASPlayerController* PC = Cast<AGASPlayerController>(ActorInfo->PlayerController.Get());
			PC->LookAtCursor();
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
