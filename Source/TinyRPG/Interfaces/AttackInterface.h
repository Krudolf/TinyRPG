﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TINYRPG_API IAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Attack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool CanAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UAnimMontage* PlayAttackMontage();
};
