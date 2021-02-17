// Fill out your copyright notice in the Description page of Project Settings.


#include "CuttableTree.h"
#include "TinyRPG/Actors/TreeLog.h"

// Sets default values
ACuttableTree::ACuttableTree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ACuttableTree::BeginPlay()
{
	Super::BeginPlay();

	if (TreeLogClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TeeLogClass is nos setted in %s"), *GetName());
	}
	
}

// Called every frame
void ACuttableTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACuttableTree::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Health = FMath::Max(Health - DamageAmount, 0.f);

	UE_LOG(LogTemp, Warning, TEXT("%s has a Health value of %f"), *GetName(), Health);

	if (Health == 0)
	{
		
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100);
		FRotator SpawnRotation(FMath::RandRange(0.f, 360.f), 0, FMath::RandRange(0.f, 360.f));
		APickUpActor* TreeLog = GetWorld()->SpawnActor<APickUpActor>(TreeLogClass, SpawnLocation, SpawnRotation);
		
		Destroy();
	}

	return DamageAmount;
}
