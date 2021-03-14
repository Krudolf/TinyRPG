// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestLogComponent.generated.h"


class AQuestBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TINYRPG_API UQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestLogComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AQuestBase*> Quests;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AQuestBase* ActiveQuest;

	UFUNCTION(BlueprintCallable)
	void AddQuestToLog(AQuestBase* Quest);
		
};
