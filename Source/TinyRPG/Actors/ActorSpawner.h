// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawner.generated.h"

class AAnimal;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnNewActor);

UCLASS()
class TINYRPG_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FSpawnNewActor OnSpawnNewActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 30.f;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneRoot;
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UBoxComponent* SpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAnimal> ActorToSpawnClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentAliveActors = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSpawnQuantity = 2;

	UPROPERTY()
	FTimerHandle CheckSpawnAllActorsHandle;
	
private:
	UFUNCTION(BlueprintCallable)
	void SpawnActor();

	UFUNCTION(BlueprintCallable)
	void SpawnAllActors();

	UFUNCTION()
	void ManageSpawnNewActor();
};
