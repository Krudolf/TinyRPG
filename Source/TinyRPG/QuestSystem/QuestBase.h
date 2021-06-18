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

	void Init(class FQuestData* QuestData);

	UFUNCTION(CallInEditor)
    void OrganiseQuestInEditor();

	void ChangeMaterial() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString QuestName = TEXT("QuestName");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Description = TEXT("Description for the mission");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FObjectiveData> Objectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsStoryQuest = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RewardXP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString TurnInDescription = TEXT("Have done that for me yet?");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString TurnInTargetInteractionDescription = TEXT("");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest Base | Materials")
	UMaterial* CanAcceptMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest Base | Materials")
	UMaterial* AcceptedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* TurnInTargetActor = nullptr;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AQuestBase* PreRequisiteQuest = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCompleted = false;
	
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

	UFUNCTION(BlueprintCallable)
	void SetQuestVisibility(const bool bIsVisible) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsQuestAcceptable() const;

	
};
