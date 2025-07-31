// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GASAbilitySystemComponent.h"

void UGASAbilitySystemComponent::RecieveDamage(UGASAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	// Broadcast the damage received event
	RecievedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}