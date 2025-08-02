#pragma once

#include "CoreMinimal.h"
#include "ProjectileLoopNodeWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Projectile/ProjectileData.h"
#include "Components/Spacer.h"
#include "ProjectileLoopWidget.generated.h"

UCLASS()
class GAS_API UProjectileLoopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void PopulateHorizontalBox();
protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LoopContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ProjectileLoopNodeWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ArrowTexture;

	UPROPERTY(EditDefaultsOnly)
	TMap<EProjectileType, UTexture2D*> ProjectileTypeImageMap;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	USpacer* CreateSpacer(const float Width);

	UFUNCTION()
	void OnLoopNodeClicked(int32 NodeIndex);

	UPROPERTY()
	TArray<UProjectileLoopNodeWidget*> ProjectileLoopNodeWidgets;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmLoopButton;

	UFUNCTION()
	void OnConfirmLoopButtonClicked();
};
