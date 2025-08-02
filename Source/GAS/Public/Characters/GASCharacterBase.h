// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GAS/GAS.h"
#include "GASCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGASAbilitySystemComponent;
class UGASAttributeSetBase;
class UGameplayEffect;
class UGASGameplayAbility;

UCLASS(BlueprintType)
class GAS_API UCharacterConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGASGameplayAbility>> DefaultAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
};

UCLASS()
class GAS_API AGASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacterBase();


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "GAS|GASCharacter")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintPure, Category = "GAS|GASCharacter")
	virtual int32 GetAbilityLevel(EGASAbilityInputID AbilityInputID) const;

	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintPure, Category = "GAS|GASCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "GAS|GASCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "GAS|GASCharacter|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure, Category = "GAS|GASCharacter|Attributes")
	float GetMoveSpeedBaseValue() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GAS|GASCharacter")
	virtual void FinishDying();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	TWeakObjectPtr<UGASAbilitySystemComponent> ASC;
	TWeakObjectPtr<UGASAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS|Config")
	TObjectPtr<UCharacterConfig> CharacterConfig;

	virtual void AddChararacterAbilities();

	virtual void IntializeAttributes();

	virtual void SetHealth(float NewHealth);


};
