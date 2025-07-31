// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/GASPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"

#include "Characters/Abilities/Attributes/GASAttributeSetBase.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"

#include "Player/GASPlayerState.h"
#include "Player/GASPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AGASPlayerCharacter::AGASPlayerCharacter(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer),
StartingCameraBoomLength(300.0f),StartingCameraSocketOffset(0,0,0)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SocketOffset = FVector(60,40,0);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->FieldOfView = 80.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void AGASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (PlayerInputData->InputMappingContext == nullptr)
			{
				return;
			}
			InputSystem->AddMappingContext(PlayerInputData->InputMappingContext, 0);
		}
	}
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(PlayerInputData->MoveAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Move);
		EIC->BindAction(PlayerInputData->LookAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Look);
		EIC->BindAction(PlayerInputData->FireAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Fire);
	}
}

void AGASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();

	if(PS)
	{
		ASC = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI controller will not have a PlayerController, so we just init again
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		// this sets the attribute set base
		AttributeSetBase = PS->GetAttributeSetBase();

		IntializeAttributes(); 

		ASC->SetTagMapCount(DeadTag, 0);

		SetHealth(GetMaxHealth());
		
		AddChararacterAbilities();

		if (AGASPlayerController* PC = Cast<AGASPlayerController>(GetController()))
		{
			PC->CreateHUD();	
		}
	}
}

USpringArmComponent* AGASPlayerCharacter::GetCameraBoom() const
{
	return CameraBoom;
}

UCameraComponent* AGASPlayerCharacter::GetCamera() const
{
	return Camera;
}

void AGASPlayerCharacter::FinishDying()
{	
	Super::FinishDying();
}

void AGASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//StartingCameraBoomLength = CameraBoom->TargetArmLength;
	//StartingCameraSocketOffset = CameraBoom->GetRelativeLocation();
}

void AGASPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AGASPlayerCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
	if(!ASC.Get())
	{
		return;
	}
	if (Value.Get<bool>()) 
	{
		ASC->AbilityLocalInputPressed(InputID);

	}
	else
	{
		ASC->AbilityLocalInputReleased(InputID); 
	}
}

void AGASPlayerCharacter::Move(const FInputActionValue& Value)
{
	MovementAxis = Value.Get<FVector2D>();
	SendAbilityLocalInput(Value, static_cast<int32>(EGASAbilityInputID::Move));
}

void AGASPlayerCharacter::Look(const FInputActionValue& Value)
{
	LookAxis = Value.Get<FVector2D>();
	SendAbilityLocalInput(Value, static_cast<int32>(EGASAbilityInputID::Look));
}

void AGASPlayerCharacter::Fire(const FInputActionValue& Value)
{

	SendAbilityLocalInput(Value, static_cast<int32>(EGASAbilityInputID::Ability1));
}
