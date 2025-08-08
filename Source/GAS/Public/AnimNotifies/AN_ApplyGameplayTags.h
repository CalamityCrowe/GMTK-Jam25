// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_ApplyGameplayTags.generated.h"

/**
 *
 */
UCLASS()
class GAS_API UAN_ApplyGameplayTags : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAN_ApplyGameplayTags();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override
	{
		if (bRemoveTags == false)
			return FString("Apply Gameplay Tags");
		else
			return FString("Remove Gameplay Tags");
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
	TArray<FGameplayTag> GameplayTags; // Tags to apply when this notify is triggered

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
	bool bRemoveTags; // If true, tags will be removed instead of added
};
