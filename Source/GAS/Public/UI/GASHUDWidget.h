// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileLoopWidget.h"
#include "Blueprint/UserWidget.h"
#include "GASHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UAsyncTaskAttributeChange;
/**
 * 
 */
UCLASS()
class GAS_API UGASHUDWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	UGASHUDWidget();

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HealthBar;


	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(FGameplayAttribute Attribute, float CurrentHealth, float MaxHealth);

	void UpdateBar(float CurrentValue, float MaxValue, UProgressBar* ProgressBar, UTextBlock* TextBlock);

	void UpdateLoop();
private: 
	UAsyncTaskAttributeChange* AttributeChangeTask;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProjectileLoopWidget> ProjectileLoopWidget;
};
