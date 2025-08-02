#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Projectile/ProjectileData.h"
#include "ProjectileLoopNodeWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoopNodeClicked, int32, NodeIndex);

UCLASS()
class GAS_API UProjectileLoopNodeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UButton* GetProjectileButton() const { return ProjectileButton; }

	EProjectileType ProjectileType = EProjectileType::None;
	
	FOnLoopNodeClicked OnLoopNodeClicked;

	int32 NodeIndex = -1;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ProjectileButton;

	UFUNCTION()
	void OnLoopNodeButtonClicked();
};
