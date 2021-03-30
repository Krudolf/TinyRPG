// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuestSystem/QuestBase.h"

#include "TinyRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TINYRPG_API ATinyRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AQuestBase*> QuestsInWorld;

	UFUNCTION(BlueprintCallable)
	void RefreshQuestsVisibility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreatePlayerHUD();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void FillQuestArray();
};
