// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AsyncTaskGameplayAbilityEnded.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncAbilityEnded, UGameplayAbility*, Ability);

class UAbilitySystemComponent;
class UGameplayAbility;

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GAS_API UAsyncTaskGameplayAbilityEnded : public UBlueprintAsyncActionBase
{
public: 
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Ability")
	FAsyncAbilityEnded OnEnded;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "GAS|AsyncTask")
	static UAsyncTaskGameplayAbilityEnded* WaitForGameplayAbilityEnded(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable)
	void EndTask(); 

protected: 
	UAbilitySystemComponent* ASC;

	UFUNCTION()
	virtual void OnCallback(UGameplayAbility* Ability); 
};
