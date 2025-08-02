// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/GASEnemyCharacter.h"
#include "Enemy/GASEnemyControllerBase.h"
#include "Components/GASMeleeComponent.h"


AGASEnemyCharacter::AGASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	MeleeComponent = CreateDefaultSubobject<UGASMeleeComponent>(TEXT("MeleeComponent"));	
	if (MeleeComponent)
	{
		
		MeleeComponent->SetIsReplicated(true);
		MeleeComponent->SetIsReplicated(true);
		MeleeComponent->SetIsReplicated(true);
		MeleeComponent->SetIsReplicated(true);
		MeleeComponent->SetIsReplicated(true);
		MeleeComponent->SetIsReplicated(true);
		MeleeComponent->SetIsReplicated(true);
	}
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
