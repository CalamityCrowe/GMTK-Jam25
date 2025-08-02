// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UANS_AttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()
public: 

	UANS_AttackNotify();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

private: 

	virtual FString GetNotifyName_Implementation() const override
	{
		return FString("Attack Notify: Hit Check");
	}
};
