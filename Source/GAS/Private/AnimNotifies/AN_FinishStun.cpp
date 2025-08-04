// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_FinishStun.h"
#include "Characters/GASCharacterBase.h"

UAN_FinishStun::UAN_FinishStun()
{
	NotifyColor = FColor::Green;
}

void UAN_FinishStun::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AGASCharacterBase* tempChar = Cast<AGASCharacterBase>(MeshComp->GetOwner())) 
	{
		tempChar->FinishStun();
	}
}
