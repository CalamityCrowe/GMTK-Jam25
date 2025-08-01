// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GASMeleeComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackCompleted); 

class AGASCharacterBase;


USTRUCT(BlueprintType)
struct FGASMeleeHitInfo
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float DamageAmount;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Melee")
	float HitRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
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

	void Attack();

	mutable FOnAttackCompleted OnAttackComplete;


private: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	TArray<FGASMeleeHitInfo> MeleeHitInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AGASCharacterBase> OwnerCharacter;

	TObjectPtr<UAnimMontage> CurrentMontage;
	float CurrentRadius;
	float CurrentDamage;
	FName CurrentSocket; 

	UFUNCTION()
	virtual void FinishAttack(UAnimMontage* Montage, bool bInterrupted); 


	FOnMontageBlendingOutStarted BlendingOutDelegate;
};
