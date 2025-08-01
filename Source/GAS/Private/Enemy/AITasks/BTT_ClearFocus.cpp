// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITasks/BTT_ClearFocus.h"
#include "AIController.h"

UBTT_ClearFocus::UBTT_ClearFocus()
{
	NodeName = TEXT("Clear Focus");
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		Cont->ClearFocus(EAIFocusPriority::Gameplay);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
