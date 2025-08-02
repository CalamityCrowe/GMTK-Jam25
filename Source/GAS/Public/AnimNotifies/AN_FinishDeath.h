// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_FinishDeath.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UAN_FinishDeath : public UAnimNotify
{
	GENERATED_BODY()
	
public: 
	UAN_FinishDeath(); 
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** Returns the name of this notify */
	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("FinishDeath");
	}
};
