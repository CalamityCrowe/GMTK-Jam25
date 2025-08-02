// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProjectileLoopNodeWidget.h"

void UProjectileLoopNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ProjectileButton->OnClicked.AddDynamic(this, &UProjectileLoopNodeWidget::OnLoopNodeButtonClicked);
}

void UProjectileLoopNodeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ProjectileButton->OnClicked.RemoveAll(this);
}

void UProjectileLoopNodeWidget::OnLoopNodeButtonClicked()
{
	if (OnLoopNodeClicked.IsBound())
	{
		OnLoopNodeClicked.Broadcast(NodeIndex);
	}
}
