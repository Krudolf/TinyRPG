// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "TinyRPG/Characters/TinyRPGCharacter.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/QuestSystem/QuestBase.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ATinyRPGCharacter>(GetOwner());
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::AddToInventory(APickUpActor* ActorPickUp)
{
	if (Inventory.Num() == MaxInventoryCapacity)
	{
		return;
	}

	if(ActorPickUp->IsStackable())
	{
		FInventoryItemStack& ItemStackFound = *(Inventory.FindByPredicate([&](FInventoryItemStack const ItemStack)
		    {
		        return ItemStack.Name == ActorPickUp->GetPickUpName() && ItemStack.Quantity < ActorPickUp->GetMaxStack();
		    }
		));
	
		if (&ItemStackFound != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found stack: %s"), *ItemStackFound.Name);
			++ItemStackFound.Quantity;
		}
		else
		{
			AddNewObjectToInventory(ActorPickUp);
		}
	}
	else
	{
		AddNewObjectToInventory(ActorPickUp);
	}
	
	ActorPickUp->Collected();

	OnUpdateInventory.Broadcast(Inventory);
}

void UInventoryComponent::UpdateInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryComponent->UpdateInventory()"));
}

void UInventoryComponent::AddNewObjectToInventory(APickUpActor* ActorPickUp)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stack NOT found, creating it"));
	FInventoryItemStack ItemStack;
	ItemStack.Name = ActorPickUp->GetPickUpName();
	ItemStack.PickUpActor = ActorPickUp;
	ItemStack.Quantity = 1;
	ItemStack.Image = ActorPickUp->GetImage();

	Inventory.Add(ItemStack);
}
