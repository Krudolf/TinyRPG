// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGCharacter.h"

#include "DrawDebugHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "TinyRPG/PlayerControllers/TinyRPGPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/TinyRPGGameModeBase.h"
#include "TinyRPG/AbilitySystem/TinyRPGAbilitySystemComponent.h"
#include "TinyRPG/AbilitySystem/TinyRPGAttributeSet.h"
#include "TinyRPG/AbilitySystem/TinyRPGGameplayAbility.h"
#include "TinyRPG/ActorComponents/HealthComponent.h"
#include "TinyRPG/ActorComponents/InventoryComponent.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"
#include "TinyRPG/ActorComponents/LevelComponent.h"

#define OUT

// Sets default values
ATinyRPGCharacter::ATinyRPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	InteractionCapsule->SetupAttachment(this->GetCapsuleComponent());
	InteractionCapsule->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	InteractionCapsule->SetRelativeLocation(FVector(115.f, 0.f, 40.f));
	InteractionCapsule->SetCapsuleHalfHeight(80.f);
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	QuestLogComponent = CreateDefaultSubobject<UQuestLogComponent>(TEXT("QuestLogComponent"));

	LevelComponent = CreateDefaultSubobject<ULevelComponent>(TEXT("LevelComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetRestoreHealth(false);

	LevelComponent->OnLevelUp.AddDynamic(HealthComponent, &UHealthComponent::RestoreFullHealth);

	AbilitySystemComponent = CreateDefaultSubobject<UTinyRPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<UTinyRPGAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void ATinyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATinyRPGCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ATinyRPGCharacter::OnOverlapEnd);
	
	if(ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController()))
	{
		PlayerController->CreateInventory();
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SpawnAndAttachWeapon();
}

void ATinyRPGCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		OverlappingPickUpActor = Cast<APickUpActor>(OtherActor);
	}
}

void ATinyRPGCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingPickUpActor = nullptr;
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
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	}

	return DamageAmount;
}

// Called every frame
void ATinyRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATinyRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATinyRPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATinyRPGCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ATinyRPGCharacter::Interaction);
	// PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ATinyRPGCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ATinyRPGCharacter::ToggleInventory);
	//PlayerInputComponent->BindAction(TEXT("SelectItem"), IE_Pressed, this, &ATinyRPGCharacter::UseItem);

	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "ETinyRPGAbilityInputId", static_cast<int32>(ETinyRPGAbilityInputId::Confirm), static_cast<int32>(ETinyRPGAbilityInputId::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
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

void ATinyRPGCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ATinyRPGCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

bool ATinyRPGCharacter::GetHittedActor(FHitResult& OutHit, FVector& OutHitDirection, float Distance) const
{
	AController* CharacterController = GetController();
	FVector Location;
	FRotator Rotation;
	CharacterController->GetPlayerViewPoint(OUT Location, OUT Rotation);

	const FVector End = Location + Rotation.Vector() * Distance;
	//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 2);

	OutHitDirection = -Rotation.Vector();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	return GetWorld()->LineTraceSingleByChannel(OUT OutHit, Location, End, ECollisionChannel::ECC_PhysicsBody, CollisionParams);
}

void ATinyRPGCharacter::Interaction()
{
	if (OverlappingPickUpActor == nullptr || InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->AddToInventory(OverlappingPickUpActor);
}

// void ATinyRPGCharacter::Attack()
// {
// 	// FHitResult Hit;
// 	// FVector HitDirection;
// 	// bool bSuccess = GetHittedActor(Hit, HitDirection, HitDistance);
// 	// if (bSuccess)
// 	// {
// 	// 	DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, false, 2);
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Hitted actor %s"), *Hit.Actor->GetName());
// 	//
// 	// 	AActor* ActorHitted = Hit.GetActor();
// 	// 	if (ActorHitted != nullptr)
// 	// 	{
// 	// 		FPointDamageEvent PointDamageEvent(Damage, Hit, HitDirection, nullptr);
// 	// 		ActorHitted->TakeDamage(Damage, PointDamageEvent, GetController(), this);
// 	// 	}
// 	// }
// }

void ATinyRPGCharacter::ToggleInventory() 
{
	ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ToggleInventoryVisibility();
	}
}

void ATinyRPGCharacter::SpawnAndAttachWeapon()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);
	
	AttachWeaponToSheathSocket();
}

void ATinyRPGCharacter::AttachWeaponToHandSocket() {
	const FName RightWeaponSocketName = "RightHandWeapon";
	EquippedWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightWeaponSocketName);

	bIsWeaponSheathed = false;
}

void ATinyRPGCharacter::AttachWeaponToSheathSocket() {
	const FName SwordSheathSocket = "SwordSheath";
	EquippedWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SwordSheathSocket);

	bIsWeaponSheathed = true;
}
