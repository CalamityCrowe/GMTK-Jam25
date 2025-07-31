// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UGASAttributeSetBase::UGASAttributeSetBase()
{
}

void UGASAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		// clamps the movement speed to a range of 150 to 1000
		NewValue = FMath::Clamp<float>(NewValue,150,100);
	}
}

void UGASAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle EffectContext = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags; 
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// continue setting up the post gameplay clamp


	// checking the source actor is calid and all the other conditions


	// doing the damage calculation here if it is that attribute

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// handle health changes here after if it wasn't done via damage
		SetHealth(FMath::Clamp(GetHealth(),0.0f, GetMaxHealth()));
	}


}

