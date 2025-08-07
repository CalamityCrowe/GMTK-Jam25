// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/GASEnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AGASEnemyControllerBase::AGASEnemyControllerBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	TargetKeyName = "Target";
	POIKeyName = "PointOfInterest";
	AIStateKeyName = "State";
}

void AGASEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		SetStateAsSpawning();
	}

}

void AGASEnemyControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EAIStates AGASEnemyControllerBase::GetAIState() const
{
	return static_cast<EAIStates>(Blackboard->GetValueAsEnum(AIStateKeyName));
}

void AGASEnemyControllerBase::SetStateAsIdle()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Idle));
}

void AGASEnemyControllerBase::SetStateAsSpawning()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Spawning));
}

void AGASEnemyControllerBase::SetStateAsAttacking(UObject* AttackTarget)
{
	if (AttackTarget)
	{
		Blackboard->SetValueAsObject(TargetKeyName, AttackTarget);
	}
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Attacking));
}

void AGASEnemyControllerBase::SetStateAsDying()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Dying));
	Blackboard->ClearValue(TargetKeyName);
	Blackboard->ClearValue(POIKeyName);
	ClearFocus(EAIFocusPriority::Gameplay);
}

AActor* AGASEnemyControllerBase::GetTargetActor() const
{
	if (!Blackboard)
	{
		return nullptr;
	}
	return Cast<AActor>(Blackboard->GetValueAsObject(TargetKeyName));
}

