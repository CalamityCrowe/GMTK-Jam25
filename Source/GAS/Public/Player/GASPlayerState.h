// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GASPlayerState.generated.h"

class UGASAbilitySystemComponent;
class UGASAttributeSetBase;


/**
 * 
 */
UCLASS()
class GAS_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGASPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UGASAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetMaxHealth()const ; 

	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetHealthRegenRate() const;

	UFUNCTION(BLueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetMoveSpeed() const; 

protected: 
	UPROPERTY()
	TObjectPtr<UGASAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UGASAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag; 

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;


	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data); 
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data); 


};
