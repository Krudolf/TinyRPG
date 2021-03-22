// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TINYRPG_API ULevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelCap = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxXP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPercentage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BufferXP = 0;

    void CalculatePercentage();

	void CalculateMaxXP();

	UFUNCTION(BlueprintCallable)
	void AddXP(const float AddedXP);

	void LevelUp();
};
