// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GASMeleeComponent.h"
#include "Characters/GASCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"
// Sets default values for this component's properties
UGASMeleeComponent::UGASMeleeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGASMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	if (OwnerCharacter = Cast<AGASCharacterBase>(GetOwner()))
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UGASMeleeComponent::FinishAttack);
		}
	}
	// ...

}

void UGASMeleeComponent::HitDetect()
{
	if (CurrentSocket.IsNone())
	{
		return;
	}

	FVector Start = OwnerCharacter->GetMesh()->GetSocketLocation(CurrentSocket);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);
	TArray<FHitResult> Hits;

	UKismetSystemLibrary::SphereTraceMultiForObjects(this, Start, Start, CurrentRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, Hits, true);
#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Start, CurrentRadius, 12, FColor::Red, false, 5.f);
#endif
	if (Hits.Num() > 0)
	{
		for (FHitResult Hit : Hits)
		{
			if (Hit.GetActor() && Hit.GetActor() != OwnerCharacter)
			{
				if (AGASCharacterBase* HitCharacter = Cast<AGASCharacterBase>(Hit.GetActor()))
				{
					UAbilitySystemComponent* HitASC = HitCharacter->GetAbilitySystemComponent();
					if (!HitASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Stun")))
					{
						FGameplayEffectContextHandle EffectContext = HitASC->MakeEffectContext();
						EffectContext.AddSourceObject(this);

						FGameplayEffectSpecHandle DamageSpecHandle = HitASC->MakeOutgoingSpec(DamageEffect, 0, EffectContext);
						DamageSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), CurrentDamage);
						HitASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
					}
				}
			}
		}
	}
}

void UGASMeleeComponent::Attack()
{
	if (!OwnerCharacter || MeleeHitInfo.Num() < 1)
	{
		return;
	}
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}
	if (AnimInstance->Montage_IsPlaying(CurrentMontage))
	{
		return;
	}

	CurrentMontage = MeleeHitInfo[0].MeleeAttackMontage;
	CurrentDamage = MeleeHitInfo[0].DamageAmount;
	CurrentRadius = MeleeHitInfo[0].HitRadius;
	CurrentSocket = MeleeHitInfo[0].SocketName;

	AnimInstance->Montage_Play(CurrentMontage);
	if (BlendingOutDelegate.IsBound())
	{
		AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, CurrentMontage);
	}

}

void UGASMeleeComponent::FinishAttack(UAnimMontage* Montage, bool bInterrupted)
{
	for (const FGASMeleeHitInfo& HitInfo : MeleeHitInfo)
	{
		if (HitInfo.MeleeAttackMontage == Montage)
		{
			OnAttackComplete.Broadcast();
			OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate()->Unbind();
			break;
		}
	}
}



