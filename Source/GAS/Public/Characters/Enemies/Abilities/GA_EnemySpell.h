// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/GASGameplayAbility.h"
#include "Characters/Abilities/Tasks/GASPlayMontageAndWaitForEvent.h"
#include "Characters/Abilities/Tasks/AsyncTaskGameplayAbilityEnded.h"
#include "GA_EnemySpell.generated.h"

class AGASProjectile;
/**
 * 
 */
UCLASS()
class GAS_API UGA_EnemySpell : public UGASGameplayAbility
{
	GENERATED_BODY()
public: 
	UGA_EnemySpell();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimMontage> CastMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AGASProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName SocketName = "Muzzle";

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileSpeed = 3000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range = 1000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage = 10.0f;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventRecieved(FGameplayTag EventTag, FGameplayEventData EventData);

private: 
	UAsyncTaskGameplayAbilityEnded* AbilityEndedTask;

};
