// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANS_AttackNotify.h"
#include "Characters/Enemies/GASEnemyCharacter.h"
#include "Components/GASMeleeComponent.h"

UANS_AttackNotify::UANS_AttackNotify()
{
	//NotifyColor = FColor(255,100,100,255);
	//bShouldFireInEditor = true;
}

void UANS_AttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
}

void UANS_AttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if(AGASEnemyCharacter* CharRef = Cast<AGASEnemyCharacter>(MeshComp->GetOwner()))
	{
		if(CharRef->GetMeleeComponent())
		{
			CharRef->GetMeleeComponent()->HitDetect();
		}
	}
}

void UANS_AttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
