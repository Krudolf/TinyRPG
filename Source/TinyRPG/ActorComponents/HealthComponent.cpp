// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::ApplyDamage(const float Damage)
{
	Health = FMath::Max(Health - Damage, 0.f);

	if(bRestoreHealth)
	{
		GetWorld()->GetTimerManager().SetTimer(RestoreHealthHandle, this, &UHealthComponent::RestoreFullHealth, 30.f, false);
	}
	
	if(Health == 0.f)
	{
		bIsDead = true;
		GetWorld()->GetTimerManager().ClearTimer(RestoreHealthHandle);
	}
}

void UHealthComponent::ApplyHealing(const float Heal)
{
	Health = FMath::Min(Health + Heal, MaxHealth);
}

float UHealthComponent::GetHealthNormalized() const
{
	return Health / MaxHealth;
}

void UHealthComponent::RestoreFullHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("Restore Health for %s"), *GetOwner()->GetName());
	Health = MaxHealth;
}
