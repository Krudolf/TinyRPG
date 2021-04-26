// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "Components/CapsuleComponent.h"
#include "TinyRPG/ActorComponents/HealthComponent.h"
#include "TinyRPG/ActorComponents/LevelComponent.h"
#include "TinyRPG/Actors/ActorSpawner.h"


// Sets default values
AAnimal::AAnimal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AAnimal::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ApplyDamage(DamageAmount);
	
	if(HealthComponent->IsDead())
	{
		if(bShouldRespawn)
		{
			ManageRespawn();
		}

		if(ULevelComponent* LevelComponent = Cast<ULevelComponent>(DamageCauser->GetComponentByClass(ULevelComponent::StaticClass())))
		{
			LevelComponent->AddXP(KillXP);
		}
		
		PlayAnimDeath();
		GetController()->UnPossess();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorld()->GetTimerManager().SetTimer(AutodestructionHandle, this, &AAnimal::CallDestroy, 5.f, false);
	}
	
	return DamageAmount;
}

void AAnimal::Attack()
{
	PlayAttackAnimation();
}

void AAnimal::CallDestroy()
{
	Destroy();
}

void AAnimal::ManageRespawn() const
{
	if(AActorSpawner* Spawner = Cast<AActorSpawner>(GetOwner()))
	{
		Spawner->OnSpawnNewActor.Broadcast();
	}
}

