// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_FinishDeath.h"
#include "Characters/GASCharacterBase.h"

UAN_FinishDeath::UAN_FinishDeath()
{
	NotifyColor = FColor::Red;
}

void UAN_FinishDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);
	if(AGASCharacterBase * CharRef = Cast<AGASCharacterBase>(MeshComp->GetOwner()))
	{
		CharRef->FinishDying();
	}
}
