// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TinyRPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TINYRPG_API ATinyRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateInventory();

	void ToggleInventoryVisibility();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryClass;

	UPROPERTY()
	UUserWidget* HUD;

	UPROPERTY()
	UUserWidget* Inventory;
	
};
