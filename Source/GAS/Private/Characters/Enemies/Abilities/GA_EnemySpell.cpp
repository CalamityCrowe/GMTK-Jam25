// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Abilities/GA_EnemySpell.h"
#include "Characters/Enemies/GASEnemyCharacter.h"
#include "Characters/GASProjectile.h"
#include <Kismet/KismetMathLibrary.h>

UGA_EnemySpell::UGA_EnemySpell()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityID = EGASAbilityInputID::Ability1;
	AbilityInputID = EGASAbilityInputID::Ability1;

	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Ability1"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stun")));
}

void UGA_EnemySpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimMontage* MontageToPlay = CastMontage;

	UGASPlayMontageAndWaitForEvent* Task = UGASPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, FName("Event.Montage.SpawnProjectile"), MontageToPlay, FGameplayTagContainer(), 1.0f, FName("StartSection"), true, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_EnemySpell::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_EnemySpell::OnCompleted);
	Task->OnInterupted.AddDynamic(this, &UGA_EnemySpell::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_EnemySpell::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_EnemySpell::EventRecieved);

	Task->ReadyForActivation();


}

void UGA_EnemySpell::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_EnemySpell::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Spell cast completed!"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_EnemySpell::EventRecieved(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
	{
		AGASEnemyCharacter* Enemy = Cast<AGASEnemyCharacter>(GetOwningActorFromActorInfo());
		if (!ProjectileClass || !Enemy)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			return;
		}

		 // Ensure the enemy has a target location set

		FVector Start = Enemy->GetMesh()->GetSocketLocation(SocketName); // Adjust spawn location as needed
		FVector Forward = Enemy->GetActorForwardVector();
		FVector End = Enemy->GetAttackTargetLocation();
		FVector Direction = (End - Start).GetSafeNormal();

		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.0f);

		DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);

		FTransform MuzzleTransform = Enemy->GetMesh()->GetSocketTransform(SocketName);
		MuzzleTransform.SetRotation(SpawnRotation.Quaternion());
		MuzzleTransform.SetScale3D(FVector(1.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Enemy;

		if (AGASProjectile* Projectile = GetWorld()->SpawnActor<AGASProjectile>(ProjectileClass, MuzzleTransform, SpawnParams))
		{
			if (DamageEffectSpecHandle.Data.Get() != nullptr)
			{
				Projectile->DamageEffect = DamageEffectSpecHandle;
			}
			Projectile->InitProjectile(ProjectileSpeed, Range, Direction);
		}
	}
}

