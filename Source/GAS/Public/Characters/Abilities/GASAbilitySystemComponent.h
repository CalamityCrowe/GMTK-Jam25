// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRecievedDamageDelegate, UGASAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage); 

/**
 * 
 */
UCLASS()
class GAS_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	FRecievedDamageDelegate RecievedDamage;

	virtual void RecieveDamage(UGASAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage); 
};
