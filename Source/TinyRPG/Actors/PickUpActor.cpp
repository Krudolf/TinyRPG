// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/Characters/TinyRPGCharacter.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
	BoxCollider->AttachToComponent(SceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
