// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TinyRPGCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TINYRPG_API AMainCharacter : public ATinyRPGCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UQuestLogComponent* QuestLogComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ULevelComponent* LevelComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionDistance = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* InteractionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWeapon* EquippedWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWeaponSheathed = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
    void Interaction();

	UPROPERTY(BlueprintReadOnly)
	class APickUpActor* OverlappingPickUpActor;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	bool GetHittedActor(FHitResult& OutHit, FVector& OutHitDirection, float Distance) const;
	void ToggleInventory();
	
	void SpawnAndAttachWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void ManageDeath() override;

protected:
	void UseItem(FKey Key);

	UFUNCTION(BlueprintImplementableEvent)
	void RestoreFullHealth();
	
public:
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHandSocket();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToSheathSocket();
};
