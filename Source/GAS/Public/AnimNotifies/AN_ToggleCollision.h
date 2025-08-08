// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ToggleCollision.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UAN_ToggleCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public: 
	UAN_ToggleCollision();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// This is the name of the notify that will be displayed in the editor
	virtual FString GetNotifyName_Implementation() const override
	{
		return FString("Toggle Collision");
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bEnableCollision; // If true, collision will be enabled, if false, it will be disabled
};
