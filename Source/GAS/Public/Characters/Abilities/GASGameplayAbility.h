// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/GAS.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public: 
	UGASGameplayAbility(); 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityID = EGASAbilityInputID::None; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool bActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
