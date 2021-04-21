// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpActor.generated.h"

class USphereComponent;

UCLASS()
class TINYRPG_API APickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CallQuestOnCollectedItem() const;

	void Respawn() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Collected();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UTexture2D> Image;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInUse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RespawnTime = 30.f;
	
	UPROPERTY()
	FTimerHandle RespawnHandle;

public:
	FORCEINLINE FString GetPickUpName() const { return Name; };
	FORCEINLINE int32 GetMaxStack() const { return MaxStack; };
	FORCEINLINE bool IsStackable() const { return bIsStackable; };
	FORCEINLINE TAssetPtr<UTexture2D> GetImage() const { return Image; };
};
