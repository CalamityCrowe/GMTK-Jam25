#pragma once

#include "CoreMinimal.h"
#include "Characters/GASCharacterBase.h"
#include "Projectile/ProjectileData.h"
#include "GASPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputAction;
class UInputMappingContext;

UCLASS(BlueprintType)
class GAS_API UPlayerInputData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

};

UCLASS()
class GAS_API AGASPlayerCharacter : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	AGASPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	USpringArmComponent* GetCameraBoom() const;
	UCameraComponent* GetCamera() const;

	virtual void FinishDying() override;

	FVector2D GetMovementAxis() const { return MovementAxis; }
	FVector2D GetLookAxis() const { return LookAxis; }
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1), Category = "Loop")
	int32 LoopSize = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Loop")
	TArray<EProjectileType> AvailableProjectileTypes;

	TArray<EProjectileType> GetProjectileLoop() const { return ProjectileLoop; }
	void UpdateProjectileLoop(const TArray<EProjectileType>& InProjectileLoop) { ProjectileLoop = InProjectileLoop; }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Camera")
	float StartingCameraBoomLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Camera")
	FVector StartingCameraSocketOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Inputs")
	TObjectPtr<UPlayerInputData> PlayerInputData;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);

private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Fire(const FInputActionValue& Value);

	FVector2D MovementAxis;
	FVector2D LookAxis;

	TArray<EProjectileType> ProjectileLoop;
};
