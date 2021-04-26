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

	SpawnAllActors();

	OnSpawnNewActor.AddDynamic(this, &AActorSpawner::ManageSpawnNewActor);
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorSpawner::SpawnActor()
{
	const FVector SpawnPoint(FMath::RandPointInBox(SpawnZone->Bounds.GetBox()));
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AAnimal* NewAnimal = GetWorld()->SpawnActor<AAnimal>(ActorToSpawnClass, SpawnPoint, FRotator::ZeroRotator, SpawnParameters);
	NewAnimal->SetOwner(this);
}

void AActorSpawner::SpawnAllActors()
{
	while(CurrentAliveActors < MaxSpawnQuantity)
	{
		SpawnActor();
		CurrentAliveActors++;
	}
}

void AActorSpawner::ManageSpawnNewActor()
{
	CurrentAliveActors--;
	if(!GetWorld()->GetTimerManager().IsTimerActive(CheckSpawnAllActorsHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(CheckSpawnAllActorsHandle, this, &AActorSpawner::SpawnAllActors, SpawnTime, false);
	}
}

