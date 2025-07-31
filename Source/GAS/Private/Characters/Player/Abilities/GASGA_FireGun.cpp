// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Abilities/GASGA_FireGun.h"
#include "Characters/Player/GASPlayerCharacter.h"
#include "Characters/GASProjectile.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>

UGASGA_FireGun::UGASGA_FireGun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	AbilityID = EGASAbilityInputID::Ability1; 
	AbilityInputID = EGASAbilityInputID::Ability1;

	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Ability1"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));

	Range = 1000.0f;
	Damage = 10.0f;
}

void UGASGA_FireGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = FireMontage;

	UGASPlayMontageAndWaitForEvent* Task = UGASPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this,FName("FireGunTask"),MontageToPlay,FGameplayTagContainer(),1.0f,FName("StartSection"),true,1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGASGA_FireGun::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGASGA_FireGun::OnCompleted);
	Task->OnInterupted.AddDynamic(this, &UGASGA_FireGun::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGASGA_FireGun::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGASGA_FireGun::EventRecieved);

	Task->ReadyForActivation();
}

void UGASGA_FireGun::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGASGA_FireGun::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGASGA_FireGun::EventRecieved(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if(EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false); 
		return; 
	}

	if(GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
	{
		AGASPlayerCharacter* Player = Cast<AGASPlayerCharacter>(GetAvatarActorFromActorInfo()); 
		if (!Player || !ProjectileClass) 
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}


		// FIX LATER
		FVector Start = Player->GetCamera()->GetComponentLocation();
		FVector End = Start + Player->GetCamera()->GetForwardVector() * Range;
		FVector Direction = (End - Start).GetSafeNormal();

		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.0f);

		DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);


		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Player;

		if (AGASProjectile* Projectile = GetWorld()->SpawnActor<AGASProjectile>(ProjectileClass, Start, SpawnRotation, SpawnParams))
		{
			if (DamageEffectSpecHandle.Data.Get() != nullptr) 
				Projectile->DamageEffect = DamageEffectSpecHandle;
			Projectile->SetRangeAndSpeed(Range, Speed);
			Projectile->SetFireDirection(Direction); 
		}
	}
}
