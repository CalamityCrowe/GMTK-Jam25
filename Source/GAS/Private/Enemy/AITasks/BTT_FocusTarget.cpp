// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITasks/BTT_FocusTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Characters/GASCharacterBase.h"


UBTT_FocusTarget::UBTT_FocusTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Focus Target";
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && BlackboardComp)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
			if(AActor* act = Cast<AActor>(KeyValue))
			{
				AIController->SetFocus(act);
				EBTNodeResult::Succeeded; 
			}
		}
	}
	return EBTNodeResult::Failed;
}
