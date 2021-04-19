// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Character.h"
#include "Animal.generated.h"

class UHealthComponent;

UCLASS()
class TINYRPG_API AAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsResting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAgressive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 20.f;

	FTimerHandle AutodestructionHandle;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimDeath();
	
	UFUNCTION(BlueprintCallable)
	void Attack();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackAnimation();

private:
	UFUNCTION()
	void CallDestroy();
};
