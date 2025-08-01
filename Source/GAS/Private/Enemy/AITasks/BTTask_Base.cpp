// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITasks/BTTask_Base.h"

UBTTask_Base::UBTTask_Base()
{
}

EBTNodeResult::Type UBTTask_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}
