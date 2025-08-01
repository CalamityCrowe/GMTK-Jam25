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
}

void UGASGA_FireGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ProjectileDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGASGA_FireGun::ActivateAbility - ProjectileDataTable is not set!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimMontage* MontageToPlay = FireMontage;

	UGASPlayMontageAndWaitForEvent* Task = UGASPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this,FName("Event.Montage.SpawnProjectile"),MontageToPlay,FGameplayTagContainer(),1.0f,FName("StartSection"),true,1.0f);
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

		const FName RowName = *UEnum::GetDisplayValueAsText(Projectiles[CurrentProjectileIndex]).ToString();

		if (const FProjectileDataRow* ProjectileDataRow = ProjectileDataTable->FindRow<FProjectileDataRow>(RowName, TEXT("UGASGA_FireGun::EventRecieved")))
		{
#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Firing Projectile Type: ") + RowName.ToString() + TEXT("!"));
#endif
			
			FVector Start = Player->GetMesh()->GetSocketLocation(FName("Muzzle"));
			FVector ForwardVector = Player->GetCamera()->GetForwardVector();
			FVector End = Start + Player->GetCamera()->GetForwardVector() * (ProjectileDataRow->Speed * ProjectileDataRow->Lifetime);
			FVector Direction = (End - Start).GetSafeNormal();

			FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
			FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.0f);

			DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), ProjectileDataRow->Damage);

			FTransform MuzzleTransform = Player->GetMesh()->GetSocketTransform(FName("Muzzle"));	
			MuzzleTransform.SetRotation(SpawnRotation.Quaternion());
			MuzzleTransform.SetScale3D(FVector(1.0f));
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = Player;

			for (int32 i = 0; i < ProjectileDataRow->Bullets; ++i)
			{
				FTransform SpawnTransform = MuzzleTransform;
				FVector SpreadDirection = ForwardVector;

				if (i > 0)
				{
					SpreadDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ForwardVector, 5.0f);
					FVector SpreadOffset = SpreadDirection * ProjectileDataRow->SpreadRadius;
					SpawnTransform.SetLocation(MuzzleTransform.GetLocation() + SpreadOffset);
				}
				
				if (AGASProjectile* Projectile = GetWorld()->SpawnActor<AGASProjectile>(ProjectileClass, SpawnTransform, SpawnParams))
				{
					if (DamageEffectSpecHandle.Data.Get() != nullptr)
					{
						Projectile->DamageEffect = DamageEffectSpecHandle;
					}

					Projectile->InitProjectile(ProjectileDataRow, SpreadDirection);
				}
			}

			if (ProjectileDataRow->AvailableTime > 0.0f)
			{
				if (FirstFireTime < 0.0f)
				{
					FirstFireTime = GetWorld()->GetTimeSeconds();
				}
				else if (GetWorld()->GetTimeSeconds() >= (FirstFireTime + ProjectileDataRow->AvailableTime))
				{
					FirstFireTime = -1.0f;
					CurrentProjectileIndex = (CurrentProjectileIndex + 1) % Projectiles.Num();
				}
			}
			else
			{
				CurrentProjectileIndex = (CurrentProjectileIndex + 1) % Projectiles.Num();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UGASGA_FireGun::EventRecieved - No data found for projectile type: %s"), *RowName.ToString());
		}
	}
}
