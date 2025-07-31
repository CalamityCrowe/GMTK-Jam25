// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/GASGameplayAbility.h"
#include "GASGA_Look.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASGA_Look : public UGASGameplayAbility
{
	GENERATED_BODY()
public:
	UGASGA_Look();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
