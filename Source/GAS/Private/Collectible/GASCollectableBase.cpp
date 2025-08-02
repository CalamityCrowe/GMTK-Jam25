// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible/GASCollectableBase.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/GASPlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
AGASCollectableBase::AGASCollectableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = Collider;

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(RootComponent);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGASCollectableBase::OnCollectibleOverlap);
}

// Called when the game starts or when spawned
void AGASCollectableBase::BeginPlay()
{
	Super::BeginPlay();
	//HealthAmount = FMath::RandRange(25.0f, 37.0f); // Random health amount between 25 and 100
}

// Called every frame
void AGASCollectableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGASCollectableBase::OnCollectibleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGASPlayerCharacter* Character = Cast<AGASPlayerCharacter>(OtherActor);
	if (Character && CollectEffect)
	{

		// Apply the effect to the character

		FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this); // Add the pickup as the source object

		FGameplayEffectSpecHandle CollectEffectSpecHandle = Character->GetAbilitySystemComponent()->MakeOutgoingSpec(CollectEffect, 0, EffectContext);
		CollectEffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Pickup")), CollectibleValue); // Set the health amount to be added
		Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*CollectEffectSpecHandle.Data.Get());

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CollectParticle, GetActorLocation(), GetActorRotation());

		// Optionally destroy the pickup after applying the effect
		Destroy();
	}



}