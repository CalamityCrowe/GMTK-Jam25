// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerState.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"
#include "Characters/Player/GASPlayerCharacter.h"
#include "Player/GASPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AGASPlayerState::AGASPlayerState()
{
	// create the Ability System Component and Attribute Set
	ASC = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);

	AttributeSetBase = CreateDefaultSubobject<UGASAttributeSetBase>(TEXT("AttributeSetBase"));

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	if (ASC)
	{
		return ASC;
	}
	return nullptr;
}

UGASAttributeSetBase* AGASPlayerState::GetAttributeSetBase() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase;
	}
	return nullptr;
}

bool AGASPlayerState::IsAlive() const
{
	return GetHealth() > 0;
}

float AGASPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AGASPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AGASPlayerState::GetHealthRegenRate() const
{
	return AttributeSetBase->GetHealthRegenRate();
}

float AGASPlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (ASC)
	{
		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGASPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGASPlayerState::MaxHealthChanged);
		HealthRegenRateChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute()).AddUObject(this, &AGASPlayerState::HealthRegenRateChanged);
		MoveSpeedChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMoveSpeedAttribute()).AddUObject(this, &AGASPlayerState::MoveSpeedChange);
	}
}

void AGASPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// do a check here for the player character
	

	// if the player has died, do death stuff here

	if (!IsAlive() && !ASC->HasMatchingGameplayTag(DeadTag))
	{
		if (AGASPlayerCharacter* PlayerCharacter = Cast<AGASPlayerCharacter>(GetPawn())) 
		{
			PlayerCharacter->Die();	
		}
	}
}

void AGASPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;

	if (AGASPlayerController* PC = Cast<AGASPlayerController>(GetOwner()))
	{
		// updating UI stuff when I get to it
	}
}

void AGASPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegenRate = Data.NewValue;

	if (AGASPlayerController* PC = Cast<AGASPlayerController>(GetOwner()))
	{
		// do something with the player controller if needed
		// mainly Updating the HUD for when it needs to be updated
	}
}

void AGASPlayerState::MoveSpeedChange(const FOnAttributeChangeData& Data)
{
	float MoveSpeed = Data.NewValue;

	if (AGASPlayerController* PC = Cast<AGASPlayerController>(GetOwner()))
	{
		PC->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
}
