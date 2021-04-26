// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawner.h"

#include "Components/BoxComponent.h"
#include "TinyRPG/Characters/Animals/Animal.h"

// Sets default values
AActorSpawner::AActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	
	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));
	SpawnZone->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	while(SpawnedAnimals.Num() < MaxSpawnQuantity)
	{
		SpawnedAnimals.Add(SpawnActor());
	}

	OnSpawnNewActor.AddDynamic(this, &AActorSpawner::ManageSpawnNewActor);
	GetWorld()->GetTimerManager().SetTimer(CheckSpawnedAnimalsHandle, this, &AActorSpawner::ManageSpawnNewActor, SpawnTime, true);
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AAnimal* AActorSpawner::SpawnActor()
{
	const FVector SpawnPoint(FMath::RandPointInBox(SpawnZone->Bounds.GetBox()));
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	return GetWorld()->SpawnActor<AAnimal>(ActorToSpawnClass, SpawnPoint, FRotator::ZeroRotator, SpawnParameters);
}

void AActorSpawner::ManageSpawnNewActor()
{
	for(int32 i = 0; i < SpawnedAnimals.Num(); i++)
	{
		AAnimal* CurrentAnimal = SpawnedAnimals[i];
		if(!IsValid(CurrentAnimal))
		{
			SpawnedAnimals[i] = SpawnActor();
		}
	}
}

