// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_ToggleCollision.h"
#include "Characters/GASCharacterBase.h"
#include "Components/CapsuleComponent.h"

UAN_ToggleCollision::UAN_ToggleCollision()
{
	NotifyColor = FColor::Blue;
	bEnableCollision = true; // Default to enabling collision
}

void UAN_ToggleCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AGASCharacterBase* CharRef = Cast<AGASCharacterBase>(MeshComp->GetOwner()))
		{
			if (bEnableCollision)
			{
				CharRef->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
			else
			{
				CharRef->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}
