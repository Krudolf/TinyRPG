// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "TinyRPG/AbilitySystem/TinyRPGAttributeSet.h"
#include "TinyRPG/Interfaces/AttackInterface.h"

#include "TinyRPGCharacter.generated.h"



UCLASS()
class TINYRPG_API ATinyRPGCharacter : public ACharacter, public IAbilitySystemInterface, public IAttackInterface
{
	GENERATED_BODY()

public:
	ATinyRPGCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ATinyRPGCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	friend UTinyRPGAttributeSet;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ATinyRPGCharacter* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathAnimation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTinyRPGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UTinyRPGAttributeSet* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HitDistance = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 25.f;

	UPROPERTY(BlueprintReadWrite)
	class UAnimMontage* NextAttackAnimation;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TinyRPG")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TinyRPG")
	TArray<TSubclassOf<class UTinyRPGGameplayAbility>> DefaultAbilities;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() const;

	FORCEINLINE float GetDamage() const { return Damage; };

private:
	virtual void ManageDeath();
};
