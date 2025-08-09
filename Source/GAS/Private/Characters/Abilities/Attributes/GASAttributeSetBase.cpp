// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Characters/GASCharacterBase.h"

UGASAttributeSetBase::UGASAttributeSetBase()
{
}

void UGASAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

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
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AGASCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AGASCharacterBase>(TargetActor);
	}
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AGASCharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if(SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor)) 
			{
				SourceController = Pawn->GetController();
			}
		}

		if (SourceController) 
		{
			SourceCharacter = Cast<AGASCharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AGASCharacterBase>(SourceActor);
		}
		if (EffectContext.GetEffectCauser()) 
		{
			SourceActor = EffectContext.GetEffectCauser(); 
		}
	}

	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		FHitResult HitResult;
		if (EffectContext.GetHitResult())
		{
			HitResult = *EffectContext.GetHitResult();
		}

		const float LocalDamage = GetDamage();
		SetDamage(0.0f); // reset damage to 0 after applying it

		if (LocalDamage > 0.0f)
		{
			bool WasAlive = true;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}

			const float NewHealth = GetHealth() - LocalDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			if (!TargetCharacter->IsAlive())
			{
				WasAlive = false;
			}
			if (TargetCharacter && WasAlive) 
			{
				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();

				if(Hit)
				{
					TargetCharacter->PlayHitReact();
				}
				else
				{
					TargetCharacter->PlayHitReact();
				}


			}

		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// handle health changes here after if it wasn't done via damage
		SetHealth(FMath::Clamp(GetHealth(),0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Clamp(GetMaxHealth(), 0.0f, 100.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 150.0f, 1000.0f));
	}


}

