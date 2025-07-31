// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/GASGameplayAbility.h"
#include "Characters/Abilities/Tasks/GASPlayMontageAndWaitForEvent.h"
#include "GASGA_FireGun.generated.h"

class AGASProjectile; 

/**
 * 
 */
UCLASS()
class GAS_API UGASGA_FireGun : public UGASGameplayAbility
{
	GENERATED_BODY()
public: 
	UGASGA_FireGun();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AGASProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Properties")
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Properties")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Properties")
	float Damage;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventRecieved(FGameplayTag EventTag, FGameplayEventData EventData);
};
