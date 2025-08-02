// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITasks/BTT_DefaultAttack.h"
#include "Characters/Enemies/GASEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/GASMeleeComponent.h"


UBTT_DefaultAttack::UBTT_DefaultAttack()
{
	NodeName = TEXT("Default Attack Task");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AGASEnemyCharacter* Enemy = Cast<AGASEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Enemy->GetMeleeComponent()->Attack();
		if (Enemy->GetMeleeComponent()->OnAttackCompleted.IsBound() == false)
		{
			Enemy->GetMeleeComponent()->OnAttackCompleted.AddDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Attack Started"));
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_DefaultAttack::OnAttackFinished_Implementation()
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if(OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded); 
	}
}
