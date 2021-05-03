// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Character.h"
#include "TinyRPG/Characters/TinyRPGCharacter.h"

#include "Animal.generated.h"

UCLASS()
class TINYRPG_API AAnimal : public ATinyRPGCharacter
{
	GENERATED_BODY()

public:
	AAnimal();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* DamageBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsResting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAgressive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldRespawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KillXP = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHitted = false;

	FTimerHandle AutodestructionHandle;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void CallDestroy();

	UFUNCTION(BlueprintCallable)
	void ManageRespawn() const;
};
