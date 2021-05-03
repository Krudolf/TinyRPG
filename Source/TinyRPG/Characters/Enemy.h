// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TinyRPGCharacter.h"

#include "Enemy.generated.h"

UCLASS()
class TINYRPG_API AEnemy : public ATinyRPGCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
