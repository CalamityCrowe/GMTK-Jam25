// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GASGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UGASGameplayAbility::UGASGameplayAbility()
{
	// Set the default instancing policy
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))); 
}

void UGASGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		// Activate the ability immediately if specified
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle,false);
	}
}