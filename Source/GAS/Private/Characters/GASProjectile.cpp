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
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	Collider->SetSphereRadius(15.0f);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGASProjectile::OnOverlapBegin);
}

void AGASProjectile::InitProjectile(const FProjectileDataRow* ProjectileDataRow, const FVector& Direction)
{
	if (!ProjectileDataRow)
	{
		return;
	}

	if (!ProjectileMovement)
	{
		return;
	}

	SetLifeSpan(ProjectileDataRow->ProjectileModifiers.Lifetime);
	Speed = ProjectileDataRow->ProjectileModifiers.Speed;
	Collider->SetWorldScale3D(ProjectileDataRow->ProjectileModifiers.ProjectileScale);

	ProjectileMovement->Velocity = Direction * Speed;
}

void AGASProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void AGASProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetInstigator())
	{
		if (AGASCharacterBase* Character = Cast<AGASCharacterBase>(OtherActor))
		{
			if (UGASAbilitySystemComponent* GASC = Cast<UGASAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
			{
				FGameplayEffectContextHandle EffectContext = DamageEffect.Data.Get()->GetContext();
				EffectContext.AddSourceObject(this);
				EffectContext.AddHitResult(SweepResult);
								
				GASC->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());

				// maybe apply other effects here, like knockback or stun
				// maybe sound or particles

				Destroy();
			}
		}
	}
}

