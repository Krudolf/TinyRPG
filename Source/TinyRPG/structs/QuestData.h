#pragma once

#include "CoreMinimal.h"

#include "ObjectiveData.h"
#include "Engine/DataTable.h"

#include "QuestData.generated.h"

USTRUCT(BlueprintType)
struct TINYRPG_API FQuestData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	TArray<FObjectiveData> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	bool bIsStoryQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	bool bIsCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	int32 RewardXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString TurnInDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString TurnInTargetInteractionDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString PreRequisiteQuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString OwnerActorName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestData)
	FString TurnInTargetActorName;
};
