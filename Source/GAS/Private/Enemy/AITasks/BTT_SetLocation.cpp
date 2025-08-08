// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITasks/BTT_SetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"

UBTT_SetLocation::UBTT_SetLocation()
{
	NodeName = TEXT("Set Actor Location");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_SetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && BlackboardComp) 
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			FVector Location = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
			if (AActor* ControlledActor = AIController->GetPawn())
			{
				ControlledActor->SetActorLocation(Location);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Set Location: %s"), *Location.ToString()));
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
