// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"


#include "TinyRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/QuestSystem/QuestBase.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact_Implementation()
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
		UE_LOG(LogTemp, Error, TEXT("UQuestLogComponent NOT FOUND!"));
		return;
	}
	
	for(AQuestBase* Quest : QuestLogComponent->Quests)
	{
		Quest->OnInteractionTarget.Broadcast(this);
	}
}

