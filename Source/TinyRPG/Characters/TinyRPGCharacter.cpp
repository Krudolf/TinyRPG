// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGCharacter.h"

#include "DrawDebugHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/AbilitySystem/TinyRPGAbilitySystemComponent.h"
#include "TinyRPG/AbilitySystem/TinyRPGAttributeSet.h"
#include "TinyRPG/AbilitySystem/TinyRPGGameplayAbility.h"
#include "TinyRPG/ActorComponents/HealthComponent.h"
#include "TinyRPG/ActorComponents/InventoryComponent.h"

#define OUT

ATinyRPGCharacter::ATinyRPGCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetRestoreHealth(false);
}

void ATinyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float ATinyRPGCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(HealthComponent->IsDead())
	{
		return 0.f;
	}
	
	HealthComponent->ApplyDamage(DamageAmount);

	if(HealthComponent->IsDead())
	{
		PlayDeathAnimation();
		if(ActorHasTag("Player"))
		{
			DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		}
	}

	return DamageAmount;
}

void ATinyRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATinyRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* ATinyRPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATinyRPGCharacter::InitializeAttributes()
{
	if(AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATinyRPGCharacter::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for(TSubclassOf<UTinyRPGGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputId), this));
		}
	}
}

void ATinyRPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(ActorHasTag("Player"))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();
		GiveAbilities();
	}
}

void ATinyRPGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	
	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "ETinyRPGAbilityInputId", static_cast<int32>(ETinyRPGAbilityInputId::Confirm), static_cast<int32>(ETinyRPGAbilityInputId::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}