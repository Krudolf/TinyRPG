// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"

#include "QuestBase.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/structs/QuestData.h"

AQuestManager::AQuestManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AQuestManager::BeginPlay()
{
	for(const FName RowName : DataTable->GetRowNames())
	{
		FString ContextString;
		FQuestData* QuestData = DataTable->FindRow<FQuestData>(RowName, ContextString);
		if(QuestData)
		{
			const FTransform SpawnTransform(FRotator::ZeroRotator, FVector::ZeroVector);
			auto DeferredQuest = Cast<AQuestBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AQuestBase::StaticClass(), SpawnTransform));
			if (DeferredQuest != nullptr)
			{
				DeferredQuest->Init(QuestData);
 
				UGameplayStatics::FinishSpawningActor(DeferredQuest, SpawnTransform);
			}
		}
	}
}
