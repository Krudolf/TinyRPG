// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TinyRPG/QuestSystem/InteractInterface.h"

#include "NPC.generated.h"

UCLASS()
class TINYRPG_API ANPC : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation() override;
};
