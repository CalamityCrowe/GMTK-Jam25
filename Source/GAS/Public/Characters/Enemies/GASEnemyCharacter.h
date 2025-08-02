// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GASCharacterBase.h"
#include "GASEnemyCharacter.generated.h"

class UGASMeleeComponent;
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

	virtual void Tick(float DeltaTime) override;

	virtual void Die() override; 

	virtual void FinishDying() override;

	UGASMeleeComponent* GetMeleeComponent() const { return MeleeComponent; }

private: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Melee", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGASMeleeComponent> MeleeComponent;
};
