// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"
#include "TinyRPG/Characters/TinyRPGCharacter.h"
#include "TinyRPG/QuestSystem/QuestBase.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	ItemMesh->SetupAttachment(Collider);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(Collider);
	ParticleSystemComponent->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();

	Collider->AttachToComponent(SceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpActor::Collected()
{
	CallQuestOnCollectedItem();

	ItemMesh->SetVisibility(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleSystemComponent->Deactivate();

	GetWorld()->GetTimerManager().SetTimer(RespawnHandle, this, &APickUpActor::Respawn, RespawnTime);
}

void APickUpActor::CallQuestOnCollectedItem() const
{
	
	ATinyRPGCharacter* Player = Cast<ATinyRPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Player == nullptr)
	{
		return;
	}

	UActorComponent* ComponentFound = Player->GetComponentByClass(UQuestLogComponent::StaticClass());
	UQuestLogComponent* QuestLogComponent = Cast<UQuestLogComponent>(ComponentFound);

	if(QuestLogComponent == nullptr)
	{
		return;
	}
	
	for(AQuestBase* Quest : QuestLogComponent->Quests)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Inventory"));
		Quest->OnCollectedItem.Broadcast(this);
	}
}

void APickUpActor::Respawn() const
{
	ItemMesh->SetVisibility(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ParticleSystemComponent->Activate();
}
