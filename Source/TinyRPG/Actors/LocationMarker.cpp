// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationMarker.h"

#include "Components/SphereComponent.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"
#include "TinyRPG/Characters/TinyRPGCharacter.h"
#include "TinyRPG/QuestSystem/QuestBase.h"

// Sets default values
ALocationMarker::ALocationMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneComponent;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALocationMarker::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALocationMarker::BeginOverlap);
}

// Called every frame
void ALocationMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALocationMarker::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr)
	{
		return;
	}

	ATinyRPGCharacter* Player = Cast<ATinyRPGCharacter>(OtherActor);
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
		Quest->OnLocationReached.Broadcast(this);
	}
}

