// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "TinyRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/QuestSystem/QuestBase.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	bIsAlive = true;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Max(Health - DamageAmount, 0.f);

	//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	
	if(Health == 0.f && bIsAlive)
	{
		Kill();
	}

	return DamageAmount;
}

void AEnemy::Kill()
{
	bIsAlive = false;
	
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
		Quest->OnEnemyKilled.Broadcast(this);
	}
}

