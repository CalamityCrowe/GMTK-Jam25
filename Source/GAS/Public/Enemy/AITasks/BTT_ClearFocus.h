// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/AITasks/BTTask_Base.h"
#include "BTT_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UBTT_ClearFocus : public UBTTask_Base
{
	GENERATED_BODY()
	
public:
	UBTT_ClearFocus();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
