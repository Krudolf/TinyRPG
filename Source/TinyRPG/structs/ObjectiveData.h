// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TinyRPG/Enums/ObjectiveTypes.h"
#include "UObject/NoExportTypes.h"
#include "ObjectiveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYRPG_API FObjectiveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number;
};
