// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/Characters/TinyRPGCharacter.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APickUpActor::OnOverlapBegin);
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, 180.f, 0.f) * DeltaTime * RotationSpeed);
}

void APickUpActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		OnInteract();
	}
}

void APickUpActor::OnInteract()
{
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, *Name);

	ATinyRPGCharacter* Player = Cast<ATinyRPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Player)
	{
		Show(false);

		Player->AddToInventory(this);
	}
}

void APickUpActor::Show(bool bVisible)
{
	ECollisionEnabled::Type collision = bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

	this->SetActorTickEnabled(bVisible);

	this->ItemMesh->SetVisibility(bVisible);
	this->ItemMesh->SetCollisionEnabled(collision);

	this->BoxCollider->SetCollisionEnabled(collision);
}