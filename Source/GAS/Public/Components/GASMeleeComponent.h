// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffect.h"
#include "GASMeleeComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackCompleted); 

class AGASCharacterBase;

USTRUCT(BlueprintType)
struct FGASMeleeHitInfo
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float HitRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	TObjectPtr<UAnimMontage> MeleeAttackMontage;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_API UGASMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGASMeleeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void HitDetect();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void Attack();

	mutable FOnAttackCompleted OnAttackCompleted;


private: 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	TArray<FGASMeleeHitInfo> MeleeHitInfo;


	AGASCharacterBase* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffect;

	TObjectPtr<UAnimMontage> CurrentMontage;
	float CurrentRadius;
	float CurrentDamage;
	FName CurrentSocket; 

	UFUNCTION()
	virtual void FinishAttack(UAnimMontage* Montage, bool bInterrupted); 


	FOnMontageBlendingOutStarted BlendingOutDelegate;
};
