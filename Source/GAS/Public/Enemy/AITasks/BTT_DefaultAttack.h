// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/AITasks/BTTask_Base.h"
#include "BTT_DefaultAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UBTT_DefaultAttack : public UBTTask_Base
{
	GENERATED_BODY()
	
public: 
	UBTT_DefaultAttack(); 

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnAttackFinished();

	virtual void OnAttackFinished_Implementation();
};
