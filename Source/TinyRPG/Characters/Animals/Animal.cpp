// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "TinyRPG/ActorComponents/HealthComponent.h"
#include "TinyRPG/ActorComponents/LevelComponent.h"
#include "TinyRPG/Actors/ActorSpawner.h"


AAnimal::AAnimal()
{
	PrimaryActorTick.bCanEverTick = false;

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(RootComponent);
	
	HealthComponent->SetRestoreHealth(true);
}

void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnimal::ManageDeath()
{
	if(bShouldRespawn)
	{
		ManageRespawn();
	}

	// if(DamageCauser->ActorHasTag("Weapon"))
	// {
	// 	if(ULevelComponent* LevelComponent = Cast<ULevelComponent>(DamageCauser->GetOwner()->GetComponentByClass(ULevelComponent::StaticClass())))
	// 	{
	// 		LevelComponent->AddXP(KillXP);
	// 	}	
	// }
		
	PlayDeathAnimation();
	GetController()->UnPossess();
	DamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(AutodestructionHandle, this, &AAnimal::CallDestroy, 5.f, false);
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

