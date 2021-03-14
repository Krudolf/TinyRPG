// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"


#include "TinyRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/QuestSystem/QuestBase.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Interact_Implementation"));
	ATinyRPGCharacter* Player = Cast<ATinyRPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Player == nullptr)
	{
		return;
	}

	UActorComponent* ComponentFound = Player->GetComponentByClass(UQuestLogComponent::StaticClass());
	UQuestLogComponent* QuestLogComponent = Cast<UQuestLogComponent>(ComponentFound);

	if(QuestLogComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UQuestLogComponent NOT FOUND!"));
		return;
	}
	
	for(AQuestBase* Quest : QuestLogComponent->Quests)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Quest"));
		Quest->OnInteractionTarget.Broadcast(this);
	}
}

