// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Player/GASPlayerState.h"
#include "Characters/Abilities/Tasks/AsyncTaskAttributeChange.h"
#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"

UGASHUDWidget::UGASHUDWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// Initialize any properties or default values here if needed
}

void UGASHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AGASPlayerState* PS = GetOwningPlayer()->GetPlayerState<AGASPlayerState>();
	if (PS) 
	{
		TArray<FGameplayAttribute> AttributesToListenFor;
		AttributesToListenFor.Add(PS->GetAttributeSetBase()->GetHealthAttribute());
		AttributeChangeTask = UAsyncTaskAttributeChange::ListenForAttributesChange(PS->GetAbilitySystemComponent(), AttributesToListenFor); 
		if (AttributeChangeTask)
		{
			AttributeChangeTask->OnAttributeChanged.AddDynamic(this, &UGASHUDWidget::UpdateHealthBar);
		}
	}
}

void UGASHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (AttributeChangeTask)
	{
		AttributeChangeTask->EndTask();
		AttributeChangeTask = nullptr;
	}
}

void UGASHUDWidget::UpdateHealthBar(FGameplayAttribute Attribute,float oldHealth, float newHealth)
{
	if (Attribute != UGASAttributeSetBase::GetHealthAttribute())
	{
		// If the attribute is not health, do nothing
		return;
	}
	float Health = GetOwningPlayer()->GetPlayerState<AGASPlayerState>()->GetAttributeSetBase()->GetHealth();
	float MaxHealth = GetOwningPlayer()->GetPlayerState<AGASPlayerState>()->GetAttributeSetBase()->GetMaxHealth();

	UpdateBar(Health, MaxHealth, HealthBar, HealthText);

}

void UGASHUDWidget::UpdateBar(float CurrentValue, float MaxValue, UProgressBar* ProgressBar, UTextBlock* TextBlock)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(CurrentValue / MaxValue);
	}

	if (TextBlock)
	{
		FString Text = FString::Printf(TEXT("%.1f / %.1f"), CurrentValue, MaxValue);
		TextBlock->SetText(FText::FromString(Text));
	}
}

