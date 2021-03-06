// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGCharacter.h"

#include "DrawDebugHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/AbilitySystem/TinyRPGAbilitySystemComponent.h"
#include "TinyRPG/AbilitySystem/TinyRPGAttributeSet.h"
#include "TinyRPG/AbilitySystem/TinyRPGGameplayAbility.h"
#include "TinyRPG/ActorComponents/InventoryComponent.h"

#define OUT

ATinyRPGCharacter::ATinyRPGCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UTinyRPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<UTinyRPGAttributeSet>(TEXT("Attributes"));
}

void ATinyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATinyRPGCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ATinyRPGCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ATinyRPGCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(DeltaValue, EventTags);
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

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
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

float ATinyRPGCharacter::GetHealth() const
{
	return Attributes->GetHealth();
}

float ATinyRPGCharacter::GetMaxHealth() const
{
	return Attributes->GetMaxHealth();
}

void ATinyRPGCharacter::ManageDeath()
{
}
