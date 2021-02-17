// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#define OUT

void ATinyRPGPlayerController::CreateInventory()
{
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), OUT FoundWidgets, InventoryClass, false);

		if (FoundWidgets.Num() > 0)
		{
			Inventory = FoundWidgets[0];
			Inventory->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ATinyRPGPlayerController::ToggleInventoryVisibility()
{
	if (Inventory == nullptr)
	{
		return;
	}
	ESlateVisibility Visibility = Inventory->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	Inventory->SetVisibility(Visibility);

	bool bInventoryVisible = Inventory->IsVisible();

	bShowMouseCursor = bInventoryVisible;
	bEnableClickEvents = bInventoryVisible;
	bEnableMouseOverEvents = bInventoryVisible;

	SetIgnoreLookInput(bInventoryVisible);

	if (!bInventoryVisible) 
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	}
}
