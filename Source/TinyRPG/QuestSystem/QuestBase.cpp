// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBase.h"

// Sets default values
AQuestBase::AQuestBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestBase::BeginPlay()
{
	Super::BeginPlay();

	OnLocationReached.AddDynamic(this, &AQuestBase::CheckLocationObjective);
	OnInteractionTarget.AddDynamic(this, &AQuestBase::CheckInteractionObjective);
}

// Called every frame
void AQuestBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestBase::OrganiseQuestInEditor()
{
	AActor* Parent = GetAttachParentActor();
	if(Parent == nullptr)
	{
		return;
	}

	this->SetActorLocation(Parent->GetActorLocation());
}

void AQuestBase::CheckLocationObjective(const ALocationMarker* LocationMarker)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Location reached"));
	int32 SavePosition = -1;
	for(int32 Position = 0; Position < Objectives.Num(); Position++)
	{
		if(!Objectives[Position].bIsComplete && Objectives[Position].Target == LocationMarker)
		{
			SavePosition = Position;
		}
	}

	if(SavePosition != -1)
	{
		FObjectiveData Objective = Objectives[SavePosition];
		Objective.bIsComplete = true;
		Objectives[SavePosition] = Objective;
		
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("NeedToUpdateUI"));
		RefreshUI();
	}
}

void AQuestBase::CheckInteractionObjective(const AActor* InteractionTarget)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("CheckInteractionObjective"));
	int32 SavePosition = -1;
	for(int32 Position = 0; Position < Objectives.Num(); Position++)
	{
		if(!Objectives[Position].bIsComplete && Objectives[Position].Target == InteractionTarget)
		{
			SavePosition = Position;
		}
	}

	if(SavePosition != -1)
	{
		FObjectiveData Objective = Objectives[SavePosition];
		Objective.bIsComplete = true;
		Objectives[SavePosition] = Objective;
		
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("NeedToUpdateUI"));
		RefreshUI();
	}
}

