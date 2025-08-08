// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_ApplyGameplayTags.h"
#include "Characters/GASCharacterBase.h"
#include "AbilitySystemComponent.h"

UAN_ApplyGameplayTags::UAN_ApplyGameplayTags()
{
	NotifyColor = FColor::Cyan; // Set a default color for the notify
	bRemoveTags = false; // Default to adding tags
}

void UAN_ApplyGameplayTags::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AGASCharacterBase* Character = Cast<AGASCharacterBase>(MeshComp->GetOwner()))
		{
			// Apply gameplay tags to the character
			for (const FGameplayTag& Tag : GameplayTags)
			{
				if(bRemoveTags == false)
				{
					if (!Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag))
					{
						Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Tag);
					}
				}
				else
				{
					if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag))
					{
						Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag);
					}
				}
			}
		}
	}
}
