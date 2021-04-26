// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULevelComponent::ULevelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULevelComponent::BeginPlay()
{
	Super::BeginPlay();

	if(EmitterLevelUp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EmitterLevelUp not set for %s in %s"), *GetName(), *GetOwner()->GetName());
	}

	CalculatePercentage();
}


// Called every frame
void ULevelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentXP = FMath::CeilToInt(FMath::FInterpTo(CurrentXP, BufferXP, DeltaTime, 5.f));
	CalculatePercentage();

	if(CurrentXP >= CurrentMaxXP)
	{
		LevelUp();
	}
}

void ULevelComponent::CalculatePercentage()
{
	CurrentPercentage = CurrentXP / CurrentMaxXP;
}

void ULevelComponent::CalculateMaxXP()
{
	CurrentMaxXP = FMath::CeilToInt(CurrentMaxXP * 1.7);
}

void ULevelComponent::AddXP(const float AddedXP)
{
	BufferXP += AddedXP;
}

void ULevelComponent::LevelUp()
{
	if(CurrentLevel >= LevelCap)
	{
		CurrentXP = CurrentMaxXP;
		BufferXP = CurrentMaxXP;
	}
	else
	{
		CurrentLevel++;
		const float RemainingXP = BufferXP - CurrentMaxXP;
		CalculateMaxXP();
		CurrentXP = 0;
		BufferXP = 0;
		AddXP(RemainingXP);

		OnLevelUp.Broadcast();
		
		const FVector SpawnLocation = FVector::UpVector * -20 + GetOwner()->GetActorLocation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterLevelUp, SpawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);
	}
}

