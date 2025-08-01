// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/Tasks/WaitReceiveDamage.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"


UWaitReceiveDamage::UWaitReceiveDamage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TriggerOnce = false;
}


void UWaitReceiveDamage::Activate()
{
	UGASAbilitySystemComponent* ASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);

	if (ASC) 
	{
		ASC->RecievedDamage.AddDynamic(this, &UWaitReceiveDamage::OnDamageReceived);
	}
}

void UWaitReceiveDamage::OnDamageReceived(UGASAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	if (ShouldBroadcastAbilityTaskDelegates()) 
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}
	if (TriggerOnce)
	{
		EndTask();
	} 
}


UWaitReceiveDamage* UWaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool InTriggerOnce)
{
	UWaitReceiveDamage* MyObj = NewAbilityTask<UWaitReceiveDamage>(OwningAbility);
	MyObj->TriggerOnce = InTriggerOnce;
	return nullptr;
}

void UWaitReceiveDamage::OnDestroy(bool AbilityEnded)
{
	UGASAbilitySystemComponent* ASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);

	if (ASC)
	{
		ASC->RecievedDamage.RemoveDynamic(this, &UWaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityEnded);
}
