// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "TinyRPG/ActorComponents/HealthComponent.h"


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
		PlayAnimDeath();
		GetWorld()->GetTimerManager().SetTimer(AutodestructionHandle, this, &AAnimal::CallDestroy, 5.f, false);
		GetController()->UnPossess();
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

