// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#define OUT

void ATinyRPGPlayerController::CreateInventory()
{
	Inventory = CreateWidget(this, InventoryClass);
	if (Inventory != nullptr)
	{
		Inventory->AddToViewport();
		Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATinyRPGPlayerController::ToggleInventoryVisibility()
{
	if (Inventory == nullptr)
	{
		return;
	}
	const ESlateVisibility Visibility = Inventory->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	Inventory->SetVisibility(Visibility);

	const bool bInventoryVisible = Inventory->IsVisible();

	bShowMouseCursor = bInventoryVisible;
	bEnableClickEvents = bInventoryVisible;
	bEnableMouseOverEvents = bInventoryVisible;

	SetIgnoreLookInput(bInventoryVisible);

	if (!bInventoryVisible) 
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	}
}
