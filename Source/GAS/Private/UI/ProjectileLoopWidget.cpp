#include "UI/ProjectileLoopWidget.h"

#include "Characters/Player/GASPlayerCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ProjectileLoopNodeWidget.h"

void UProjectileLoopWidget::PopulateHorizontalBox()
{
	if (!LoopContainer || !ProjectileLoopNodeWidgetClass || !ArrowTexture)
	{
		return;
	}

	LoopContainer->ClearChildren();	
	ProjectileLoopNodeWidgets.Empty();
	
	if (const AGASPlayerCharacter* PlayerCharacter = Cast<AGASPlayerCharacter>(GetOwningPlayerPawn()))
	{
		const int32 LoopSize = PlayerCharacter->LoopSize;

		for (int32 i = 0; i < LoopSize; ++i)
		{
			if (USizeBox* SizeBox = NewObject<USizeBox>(this, USizeBox::StaticClass()))
			{
				SizeBox->SetWidthOverride(150.0f);
				SizeBox->SetHeightOverride(150.0f);

				if (UProjectileLoopNodeWidget* LoopNode = CreateWidget<UProjectileLoopNodeWidget>(this, ProjectileLoopNodeWidgetClass))
				{
					ProjectileLoopNodeWidgets.Add(LoopNode);
					LoopNode->NodeIndex = i;

					FSlateBrush Brush;
					Brush.SetResourceObject(ProjectileTypeImageMap.FindRef(EProjectileType::None));

					Brush.ImageSize = FVector2D(150.f, 150.f);

					FButtonStyle NewStyle = LoopNode->GetProjectileButton()->GetStyle();

					NewStyle.SetNormal(Brush);
					NewStyle.SetHovered(Brush);
					NewStyle.SetPressed(Brush);

					LoopNode->GetProjectileButton()->SetStyle(NewStyle);
					
					LoopNode->OnLoopNodeClicked.AddDynamic(this, &UProjectileLoopWidget::OnLoopNodeClicked);

					if (UHorizontalBoxSlot* NodeSlot = LoopContainer->AddChildToHorizontalBox(SizeBox))
					{
						NodeSlot->SetPadding(FMargin(5.0f));
						NodeSlot->SetVerticalAlignment(VAlign_Center);
					}

					SizeBox->AddChild(LoopNode);
				}
			}

			if (i < LoopSize - 1)
			{
				if (USpacer* Spacer = CreateSpacer(5.0f))
				{
					UHorizontalBoxSlot* SpacerSlot = LoopContainer->AddChildToHorizontalBox(Spacer);
				}

				if (USizeBox* SizeBox = NewObject<USizeBox>(this, USizeBox::StaticClass()))
				{
					SizeBox->SetWidthOverride(150.0f);
					SizeBox->SetHeightOverride(150.0f);

					if (UImage* ArrowImage = NewObject<UImage>(this, UImage::StaticClass()))
					{
						ArrowImage->SetBrushFromTexture(ArrowTexture, false);
						if (UHorizontalBoxSlot* ArrowSlot = LoopContainer->AddChildToHorizontalBox(SizeBox))
						{
							ArrowSlot->SetPadding(FMargin(5.0f));
							ArrowSlot->SetVerticalAlignment(VAlign_Center);
						}

						SizeBox->AddChild(ArrowImage);
					}
				}
			}

			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(LoopContainer->Slot))
			{
				int32 ChildrenCount = LoopContainer->GetChildrenCount();

				int32 NodesAdded = i + 1;
				int32 ArrowsAdded = (i < LoopSize - 1) ? i : (LoopSize - 1);
				int32 SpacersAdded = ArrowsAdded + (i < LoopSize - 1 ? 1 : 0);

				const float NodeWidth = 150.0f;
				const float NodePadding = 5.0f * 2;
				const float SpacerWidth = 5.0f;
				const float ArrowWidth = 150.0f;
				const float ArrowPadding = 5.0f * 2; 

				float TotalNodesWidth = NodesAdded * (NodeWidth + NodePadding);
				float TotalArrowsWidth = ArrowsAdded * (ArrowWidth + ArrowPadding);
				float TotalSpacerWidth = SpacersAdded * SpacerWidth;

				float TotalWidth = TotalNodesWidth + TotalArrowsWidth + TotalSpacerWidth;

				CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
				CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

				const float TweakOffset = 65.0f;

				CanvasSlot->SetPosition(FVector2D(-TotalWidth * 0.5f + TweakOffset, 0.f));
			}
		}


		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->bShowMouseCursor = true;

			UGameplayStatics::SetGamePaused(GetWorld(), true);
			
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(this->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PC->SetInputMode(InputMode);
		}
	}
}

void UProjectileLoopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmLoopButton->OnClicked.AddDynamic(this, &UProjectileLoopWidget::OnConfirmLoopButtonClicked);
}

void UProjectileLoopWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ConfirmLoopButton->OnClicked.RemoveAll(this);
}

USpacer* UProjectileLoopWidget::CreateSpacer(const float Width)
{
	if (USpacer* Spacer = NewObject<USpacer>(this, USpacer::StaticClass()))
	{
		Spacer->SetSize(FVector2D(Width, Spacer->GetSize().Y));
		return Spacer;
	}

	return nullptr;
}

void UProjectileLoopWidget::OnLoopNodeClicked(int32 NodeIndex)
{
	if (UProjectileLoopNodeWidget* ClickedNode = ProjectileLoopNodeWidgets[NodeIndex])
	{
		EProjectileType CurrentType = ClickedNode->ProjectileType;

		TArray<EProjectileType> Keys;
		ProjectileTypeImageMap.GetKeys(Keys);

		int32 CurrentIndex = Keys.IndexOfByKey(CurrentType);
		int32 NextIndex = (CurrentIndex + 1) % Keys.Num();

		while (Keys[NextIndex] == EProjectileType::None)
		{
			NextIndex = (NextIndex + 1) % Keys.Num();
		}
		
		EProjectileType NextType = Keys[NextIndex];

		ClickedNode->ProjectileType = NextType;

		if (UTexture2D* NextTex = ProjectileTypeImageMap.FindRef(NextType))
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(NextTex);

			Brush.ImageSize = FVector2D(150.f, 150.f);

			FButtonStyle NewStyle = ClickedNode->GetProjectileButton()->GetStyle();

			NewStyle.SetNormal(Brush);
			NewStyle.SetHovered(Brush);
			NewStyle.SetPressed(Brush);

			ClickedNode->GetProjectileButton()->SetStyle(NewStyle);
		}
	}
}

void UProjectileLoopWidget::OnConfirmLoopButtonClicked()
{
	bool bAllSlotsChosen = true;
	TArray<EProjectileType> ProjectilesArray;

	for (int32 i = 0; i < ProjectileLoopNodeWidgets.Num(); ++i)
	{
		ProjectilesArray.Add(ProjectileLoopNodeWidgets[i]->ProjectileType);
		
		if (ProjectileLoopNodeWidgets[i]->ProjectileType == EProjectileType::None)
		{
			bAllSlotsChosen = false;
			break;
		}
	}

	if (!bAllSlotsChosen)
	{
		return;
	}
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->bShowMouseCursor = false;

		UGameplayStatics::SetGamePaused(GetWorld(), false);

		FInputModeGameOnly InputMode;

		PC->SetInputMode(InputMode);

		SetVisibility(ESlateVisibility::Hidden);

		if (AGASPlayerCharacter* PlayerCharacter = Cast<AGASPlayerCharacter>(PC->GetPawn()))
		{
			PlayerCharacter->UpdateProjectileLoop(ProjectilesArray);
		}
	}
}
