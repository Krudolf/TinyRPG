// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"


#include "TinyRPGCharacter.generated.h"

class UQuestLogComponent;
class APickUpActor;
class UInventoryComponent;
class ULevelComponent;
class UHealthComponent;

UCLASS()
class TINYRPG_API ATinyRPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATinyRPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UQuestLogComponent* QuestLogComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ULevelComponent* LevelComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionDistance = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HitDistance = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* InteractionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
    void Interaction();

	UPROPERTY(BlueprintReadOnly)
	APickUpActor* OverlapingPickUpActor;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	bool GetHittedActor(FHitResult& OutHit, FVector& OutHitDirection, float Distance);
	void Hit();
	void ToggleInventory();

protected:
	void UseItem(FKey Key);
};
