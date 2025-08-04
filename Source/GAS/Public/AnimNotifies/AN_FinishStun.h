// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_FinishStun.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UAN_FinishStun : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAN_FinishStun();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// This is the name of the notify that will be displayed in the editor
	virtual FString GetNotifyName_Implementation() const override
	{
		return FString("Finish Stun");
	}

};
