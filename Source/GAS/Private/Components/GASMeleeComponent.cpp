#include "Components/GASMeleeComponent.h"
#include "Characters/GASCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"

UGASMeleeComponent::UGASMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UGASMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AGASCharacterBase>(GetOwner());
	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			BlendingOutDelegate.BindUObject(this, &UGASMeleeComponent::FinishAttack);
		}
	}

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

	int Index = FMath::RandRange(0, MeleeHitInfo.Num() - 1);

	CurrentMontage = MeleeHitInfo[Index].MeleeAttackMontage;
	CurrentDamage = MeleeHitInfo[Index].DamageAmount;
	CurrentRadius = MeleeHitInfo[Index].HitRadius;
	CurrentSocket = MeleeHitInfo[Index].SocketName;

	AnimInstance->Montage_Play(CurrentMontage);
	if (BlendingOutDelegate.IsBound())
	{
		AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, CurrentMontage);
	}

}

void UGASMeleeComponent::FinishAttack(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == CurrentMontage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Attack Finished"));
		OnAttackCompleted.Broadcast();
	}
}



