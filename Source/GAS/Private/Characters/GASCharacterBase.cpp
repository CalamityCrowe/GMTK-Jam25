// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GASCharacterBase.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"
#include "Characters/Abilities/GASGameplayAbility.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameframework/CharacterMovementComponent.h"


AGASCharacterBase::AGASCharacterBase() :Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return ASC.Get();
}

bool AGASCharacterBase::IsAlive() const
{
	return GetHealth()>0.0f;
}

int32 AGASCharacterBase::GetAbilityLevel(EGASAbilityInputID AbilityInputID) const
{
	return 1;
}

void AGASCharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !ASC.IsValid())
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove; 
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterConfig->DefaultAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
	{
		ASC->ClearAbility(AbilitiesToRemove[i]);
	}
}

float AGASCharacterBase::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float AGASCharacterBase::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float AGASCharacterBase::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeed();
	}
	return 0.0f;
}

float AGASCharacterBase::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase.Get())->GetBaseValue();
	}
	return 0.0f;
}

void AGASCharacterBase::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// do a delegate here for when the character dies?

	if (ASC.IsValid())
	{
		ASC->CancelAllAbilities(); 
		
		StopAnimMontage(HitMontage); 

		if(DeathMontage)
		{
			PlayAnimMontage(DeathMontage);
		}
		else
		{
			FinishDying();
		}
	}
}

void AGASCharacterBase::FinishDying()
{
	Destroy();
}

void AGASCharacterBase::FinishStun()
{
	if (ASC.IsValid()) 
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Stun")))
		{
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Stun"));
		}
	}
}

void AGASCharacterBase::PlayHitReact()
{
	if(HitMontage)
	{
		PlayAnimMontage(HitMontage);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit React Played"));
	}
}

// Called when the game starts or when spawned
void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	MoveDirections.Forward = GetActorForwardVector();
	MoveDirections.Right = GetActorRightVector();
}

void AGASCharacterBase::AddChararacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !ASC.IsValid()) 
	{
		return;
	}
	

	for (TSubclassOf<UGASGameplayAbility> Ability : CharacterConfig->DefaultAbilities)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability, GetAbilityLevel(Ability.GetDefaultObject()->AbilityID), static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void AGASCharacterBase::IntializeAttributes()
{
	if (!ASC.IsValid()) 
	{
		return;
	}	

	if(!CharacterConfig->DefaultAttributes)
	{
		UE_LOG(LogTemp, Warning, TEXT("DefaultAttributes is not set for %s"), *GetName());
		return; 
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(CharacterConfig->DefaultAttributes, 1, EffectContext);
	if (EffectSpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void AGASCharacterBase::SetHealth(float NewHealth)
{
	if(AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(NewHealth);
	}
}

void AGASCharacterBase::SetMoveSpeed(float NewMoveSpeed)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetMoveSpeed(NewMoveSpeed);
	}
}




