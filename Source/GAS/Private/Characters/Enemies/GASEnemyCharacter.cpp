// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/GASEnemyCharacter.h"
#include "Enemy/GASEnemyControllerBase.h"
#include "Components/GASMeleeComponent.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"

AGASEnemyCharacter::AGASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	MeleeComponent = CreateDefaultSubobject<UGASMeleeComponent>(TEXT("MeleeComponent"));	

	HardRefASC = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("ASC"));
	HardRefASC->SetIsReplicated(true);
	ASC = HardRefASC;

	HardRefAttributeSet = CreateDefaultSubobject<UGASAttributeSetBase>(TEXT("AttributeSet"));
	AttributeSetBase = HardRefAttributeSet;

}

void AGASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ASC.IsValid()) 
	{
		ASC->InitAbilityActorInfo(this, this);
		IntializeAttributes();

		SetHealth(GetMaxHealth()); // Set initial health, can be modified as needed

		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGASEnemyCharacter::OnHealthChanged);
	}
}

void AGASEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void AGASEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Enemy Health: %f"), GetHealth()));
}

void AGASEnemyCharacter::Die()
{
	Super::Die();
	if(AGASEnemyControllerBase* EC = Cast<AGASEnemyControllerBase>(GetController()))
	{
		EC->SetStateAsDying();
		//EC->UnPossess(); 
	}
}

void AGASEnemyCharacter::FinishDying()
{
	Super::FinishDying();
}

void AGASEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float NewHealth = Data.NewValue;

	if (!IsAlive() && !ASC->HasMatchingGameplayTag(DeadTag))
	{
		Die();
	}
}
