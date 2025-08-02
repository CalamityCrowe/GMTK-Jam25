// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "GASCollectableBase.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UCapsuleComponent;

UCLASS()
class GAS_API AGASCollectableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGASCollectableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Tick(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Niagara System",meta =  (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> CollectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Niagara System", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> ParticleComponent; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent> Collider;

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Gameplay Effect", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> CollectEffect; 

	UFUNCTION()
	void OnCollectibleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Collectible", meta = (AllowPrivateAccess = true))
	float CollectibleValue = 10.0f;
};
