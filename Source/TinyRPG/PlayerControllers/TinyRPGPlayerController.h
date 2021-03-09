// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CanvasPanel.h"
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

	void CreatePickUpWidget();

	void RemovePickUpWidget();

private:
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;*/

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PickUpClass;

	UPROPERTY()
	UUserWidget* PickUp;

	UPROPERTY()
	UUserWidget* Inventory;

public:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCanvasPanel* HUDCanvas;*/
};
