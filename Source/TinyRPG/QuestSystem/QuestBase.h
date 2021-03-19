// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyRPG/Actors/LocationMarker.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "TinyRPG/structs/ObjectiveData.h"
#include "TinyRPG/Characters/Enemy.h"

#include "QuestBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocationReachedDelegate, const ALocationMarker*, LocationMarker);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionTargetDelegate, const AActor*, InteractionTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollectedItemDelegate, const APickUpActor*, CollectedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKillEnemyDelegate, const AEnemy*, KilledEnemy);

UCLASS()
class TINYRPG_API AQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor)
    void OrganiseQuestInEditor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestName = TEXT("QuestName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description = TEXT("Description for the mission");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectiveData> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStoryQuest = true;

	//Event dispatchers
	UPROPERTY(BlueprintAssignable)
	FLocationReachedDelegate OnLocationReached;

	UPROPERTY(BlueprintAssignable)
	FInteractionTargetDelegate OnInteractionTarget;

	UPROPERTY(BlueprintAssignable)
	FCollectedItemDelegate OnCollectedItem;

	UPROPERTY(BlueprintAssignable)
	FKillEnemyDelegate OnEnemyKilled;

	UFUNCTION()
    void CheckLocationObjective(const ALocationMarker* LocationMarker);

	UFUNCTION()
    void CheckInteractionObjective(const AActor* InteractionTarget);

	UFUNCTION()
	void CheckCollectItemObjective(const APickUpActor* CollectedItem);

	UFUNCTION()
	void CheckEnemyKilledObjective(const AEnemy* KilledEnemy);

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshUI();
};
