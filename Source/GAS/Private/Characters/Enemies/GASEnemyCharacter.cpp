// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/GASEnemyCharacter.h"
#include "Enemy/GASEnemyControllerBase.h"

AGASEnemyCharacter::AGASEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AGASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGASEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGASEnemyCharacter::Die()
{
	Super::Die();
	if(AGASEnemyControllerBase* EC = Cast<AGASEnemyControllerBase>(GetController()))
	{
		EC->SetStateAsDying();
		EC->UnPossess(); 
	}
}

void AGASEnemyCharacter::FinishDying()
{
	Super::FinishDying();
}
