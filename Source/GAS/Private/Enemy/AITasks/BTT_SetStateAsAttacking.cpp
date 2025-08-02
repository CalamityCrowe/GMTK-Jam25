// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITasks/BTT_SetStateAsAttacking.h"
#include "Enemy/GASEnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetStateAsAttacking::UBTT_SetStateAsAttacking()
{
	NodeName = TEXT("Set State As Attacking");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_SetStateAsAttacking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner())
	{
		if (AGASEnemyControllerBase* EnemyController = Cast<AGASEnemyControllerBase>(OwnerComp.GetAIOwner()))
		{
			EnemyController->SetStateAsAttacking(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyController->GetTargetKeyName()));
			return EBTNodeResult::Succeeded;
		}

	}

	return EBTNodeResult::Failed;
}