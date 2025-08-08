// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GASCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "GASEnemyCharacter.generated.h"

class UGASMeleeComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class GAS_API AGASEnemyCharacter : public AGASCharacterBase
{
	GENERATED_BODY()
public: 

	AGASEnemyCharacter();

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Die() override; 

	virtual void FinishDying() override;

	UGASMeleeComponent* GetMeleeComponent() const { return MeleeComponent; }

	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }


	FVector GetAttackTargetLocation() const;
private: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Melee", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGASMeleeComponent> MeleeComponent;

	class UGASAbilitySystemComponent* HardRefASC = nullptr;
	class UGASAttributeSetBase* HardRefAttributeSet = nullptr;


	FDelegateHandle HealthChangedDelegateHandle;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
