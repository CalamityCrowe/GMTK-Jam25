// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/Tasks/AsyncTaskGameplayAbilityEnded.h"
#include "AbilitySystemComponent.h"

UAsyncTaskGameplayAbilityEnded* UAsyncTaskGameplayAbilityEnded::WaitForGameplayAbilityEnded(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> Ability)
{
	if(!IsValid(AbilitySystemComponent))
	{
		return nullptr;
	}

	const UGameplayAbility* AbilityDef = Ability.GetDefaultObject();
	if (AbilityDef == nullptr) 
	{

		return nullptr;
	}

	UAsyncTaskGameplayAbilityEnded* Task = NewObject<UAsyncTaskGameplayAbilityEnded>();
	Task->ASC = AbilitySystemComponent;

	AbilitySystemComponent->AbilityEndedCallbacks.AddUObject(Task, &UAsyncTaskGameplayAbilityEnded::OnCallback);

	return Task;
}

void UAsyncTaskGameplayAbilityEnded::EndTask()
{
	if (IsValid(ASC))
	{
		SetReadyToDestroy();
		MarkAsGarbage();
	}
}

void UAsyncTaskGameplayAbilityEnded::OnCallback(UGameplayAbility* Ability)
{
	OnEnded.Broadcast(Ability);
}
