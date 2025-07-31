// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GASProjectile.h"
#include "Components/SphereComponent.h"
#include "Characters/GASCharacterBase.h"
#include "Characters/Abilities/GASAbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGASProjectile::AGASProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	Collider->SetSphereRadius(15.0f);

	Collider->OnComponentHit.AddDynamic(this, &AGASProjectile::OnHit);
}

void AGASProjectile::SetRangeAndSpeed(float InRange, float InSpeed)
{
	Range = InRange;
	Speed = InSpeed;
	float Time = Range / Speed;
	SetLifeSpan(Time);
}

void AGASProjectile::SetFireDirection(FVector Direction)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * Speed;
	}
}

// Called when the game starts or when spawned
void AGASProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AGASProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator())
	{
		if (AGASCharacterBase* Character = Cast<AGASCharacterBase>(OtherActor))
		{
			if (UGASAbilitySystemComponent* GASC = Cast<UGASAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
			{
				FGameplayEffectContextHandle EffectContext = DamageEffect.Data.Get()->GetContext();
				GASC->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());

				// maybe apply other effects here, like knockback or stun
				// maybe sound or particles

				Destroy(); 
			}
		}
	}
}


