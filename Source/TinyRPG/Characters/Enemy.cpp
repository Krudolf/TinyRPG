// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "TinyRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/QuestSystem/QuestBase.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

