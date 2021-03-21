// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBase.h"

// Sets default values
AQuestBase::AQuestBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AQuestBase::BeginPlay()
{
	Super::BeginPlay();

	OnLocationReached.AddDynamic(this, &AQuestBase::CheckLocationObjective);
	OnInteractionTarget.AddDynamic(this, &AQuestBase::CheckInteractionObjective);
	OnCollectedItem.AddDynamic(this, &AQuestBase::CheckCollectItemObjective);
	OnEnemyKilled.AddDynamic(this, &AQuestBase::CheckEnemyKilledObjective);

	if(PreRequisiteQuest != nullptr && !PreRequisiteQuest->bIsCompleted)
	{
		StaticMesh->SetVisibility(false);
	}
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
	for(int32 Position = 0; Position < Objectives.Num(); Position++)
	{
		if(!Objectives[Position].bIsComplete && Objectives[Position].Target == LocationMarker)
		{
			FObjectiveData Objective = Objectives[Position];
			Objective.bIsComplete = true;
			Objectives[Position] = Objective;

			RefreshUI();
		}
	}
}

void AQuestBase::CheckInteractionObjective(const AActor* InteractionTarget)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("CheckInteractionObjective"));
	for(int32 Position = 0; Position < Objectives.Num(); Position++)
	{
		if(!Objectives[Position].bIsComplete && Objectives[Position].Target == InteractionTarget)
		{
			FObjectiveData Objective = Objectives[Position];
			Objective.bIsComplete = true;
			Objectives[Position] = Objective;

			RefreshUI();
		}
	}
}

void AQuestBase::CheckCollectItemObjective(const APickUpActor* CollectedItem)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("CheckCollectItemObjective"));
	for(int32 Position = 0; Position < Objectives.Num(); Position++)
	{
		if(!Objectives[Position].bIsComplete && Objectives[Position].Target->GetClass() == CollectedItem->GetClass())
		{
			FObjectiveData Objective = Objectives[Position];
			Objective.CurrentNumber++;
			if(Objective.CurrentNumber >= Objective.Number)
			{
				Objective.bIsComplete = true;
			}
			Objectives[Position] = Objective;

			RefreshUI();
			//UE_LOG(LogTemp, Warning, TEXT("Collected item: %i"), Objective.CurrentNumber);
		}
	}
}

void AQuestBase::CheckEnemyKilledObjective(const AEnemy* KilledEnemy)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("CheckKilledEnemyObjective"));
	for(int32 Position = 0; Position < Objectives.Num(); Position++)
	{
		if(!Objectives[Position].bIsComplete && Objectives[Position].Target->GetClass() == KilledEnemy->GetClass())
		{
			FObjectiveData Objective = Objectives[Position];
			//TODO: CurrentNumber must be a variable from QuestBase, not from ObjectiveStruct 
			Objective.CurrentNumber++;
			if(Objective.CurrentNumber >= Objective.Number)
			{
				Objective.bIsComplete = true;
			}
			Objectives[Position] = Objective;

			RefreshUI();
			//UE_LOG(LogTemp, Warning, TEXT("Enemy killed: %i"), Objective.CurrentNumber);
		}
	}
}

void AQuestBase::SetQuestVisibility(const bool bIsVisible) const
{
	StaticMesh->SetVisibility(bIsVisible);
}

bool AQuestBase::IsQuestAcceptable() const
{
	return !this->bIsCompleted && (PreRequisiteQuest == nullptr || (PreRequisiteQuest != nullptr && PreRequisiteQuest->bIsCompleted));
}

