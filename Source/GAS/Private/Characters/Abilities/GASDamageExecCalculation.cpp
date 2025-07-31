// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GASDamageExecCalculation.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"

struct FGASDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armour); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FGASDamageStatics()
	{
		// Capture the Armour attribute from the target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASAttributeSetBase, Armour, Target, false);
		// Capture the Damage attribute from the source
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASAttributeSetBase, Damage, Source, true);
	}
};

static const FGASDamageStatics& DamageStatics()
{
	static FGASDamageStatics DStatics;
	return DStatics;
}

UGASDamageExecCalculation::UGASDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmourDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UGASDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// we grab the tags from both the target and source to determine what to do
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags; 
	EvalParams.TargetTags = TargetTags;

	float Armour = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourDef,EvalParams,Armour);
	Armour = FMath::Max<float>(Armour, 0.0f); // Ensure Armour is not negative

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvalParams, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false), 0.0f); // Add SetByCaller damage if it exists

	float UnmitigatedDamage = Damage;

	float MitigatedDamage = UnmitigatedDamage * (100 / (100 + Armour));

	if(MitigatedDamage>0)
	{
		// sets the targets damage attribute to the mitigated damage
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}

	UGASAbilitySystemComponent* TargetGASC = Cast<UGASAbilitySystemComponent>(TargetASC);
	if (TargetGASC)
	{
		UGASAbilitySystemComponent* SourceGASC = Cast<UGASAbilitySystemComponent>(SourceASC);
		// Broadcast the damage received event
		TargetGASC->RecieveDamage(SourceGASC, UnmitigatedDamage, MitigatedDamage);
	}
}
