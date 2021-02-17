// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuttableTree.generated.h"

class APickUpActor;

UCLASS()
class TINYRPG_API ACuttableTree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACuttableTree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APickUpActor> TreeLogClass;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
