// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TinyRPG/structs/InventoryItemStack.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryDelegate, const TArray<FInventoryItemStack>&, InventoryItems);

class APickUpActor;
class ATinyRPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TINYRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxInventoryCapacity = 6;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToInventory(APickUpActor* ActorPickUp);

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	UPROPERTY(BlueprintAssignable)
	FUpdateInventoryDelegate OnUpdateInventory;

private:
	UPROPERTY()
	TArray<FInventoryItemStack> Inventory;
		
	UPROPERTY()
	ATinyRPGCharacter* OwnerCharacter;

	void AddNewObjectToInventory(APickUpActor* ActorPickUp);
};
