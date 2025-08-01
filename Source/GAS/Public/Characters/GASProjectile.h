// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "GASProjectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent; 

UCLASS()
class GAS_API AGASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGASProjectile();

	void SetRangeAndSpeed(float InRange, float InSpeed); 
	void SetFireDirection(FVector Direction); 

	void InitProjectile(); 

	UPROPERTY(BlueprintReadOnly, Category = "GAS", meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	FGameplayEffectSpecHandle DamageEffect;

	UStaticMeshComponent* GetMesh() const { return Mesh; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	USphereComponent* GetCollider() const { return Collider; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	float Speed;
	float Range;

private: 
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components",meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> Collider;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
