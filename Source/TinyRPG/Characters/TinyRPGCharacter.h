// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TinyRPGCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryDelegate, const TArray<APickUpActor*>&, InventoryItems);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUseItemDelegate, APickUpActor*, ActionBarItem);

class APickUpActor;

UCLASS()
class TINYRPG_API ATinyRPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATinyRPGCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MaxInventoryCapacity = 6;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionDistance = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HitDistance = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 20.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddToInventory(APickUpActor* ActorPickUp);

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	UFUNCTION(BlueprintCallable)
	void DropToActionBar(APickUpActor* ActorPickUp, int32 MaxItems);

	UPROPERTY(BlueprintAssignable)
	FUpdateInventoryDelegate OnUpdateInventory;

	UPROPERTY(BlueprintAssignable)
	FUpdateInventoryDelegate OnUpdateActionBar;

	UPROPERTY(BlueprintAssignable)
	FUseItemDelegate OnUseItemActionBar;

	UPROPERTY(BlueprintReadOnly)
	APickUpActor* OverlapingPickUpActor;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	bool GetHittedActor(FHitResult& OutHit, FVector& OutHitDirection, float Distance);
	void Interact();
	void Hit();
	void ToggleInventory();

	TArray<APickUpActor*> Inventory;

	TArray<APickUpActor*> ActionBar;

protected:
	void UseItem(FKey Key);
};
