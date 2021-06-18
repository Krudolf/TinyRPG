// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS()
class TINYRPG_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:
	AQuestManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* DataTable;
};
