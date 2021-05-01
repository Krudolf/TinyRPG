// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TinyRPG/TinyRPG.h"

#include "TinyRPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TINYRPG_API UTinyRPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTinyRPGGameplayAbility();

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	ETinyRPGAbilityInputId AbilityInputId = ETinyRPGAbilityInputId::None;
	
};
