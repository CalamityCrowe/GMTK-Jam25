#pragma once

#include "CoreMinimal.h"
#include "ProjectileData.generated.h"

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Sniper UMETA(DisplayName = "Sniper"),
	Rocket UMETA(DisplayName = "Launcher"),
	
	None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FProjectileModifiers
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1.0f))
	float Lifetime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	FVector ProjectileScale = FVector::OneVector;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int32 Bullets = 1;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "Bullets > 1", ClampMin = 1))
	float SpreadRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly)
	int32 AvailableTime = 0;

	// Modifiers
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "AvailableTime > 0"))
	float FireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float Multishot = 1.0f;
};

USTRUCT(BlueprintType)
struct FProjectileDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FProjectileModifiers ProjectileModifiers;
};