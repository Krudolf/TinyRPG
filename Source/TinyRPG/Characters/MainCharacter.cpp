// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/TinyRPG.h"
#include "TinyRPG/AbilitySystem/TinyRPGAbilitySystemComponent.h"
#include "TinyRPG/AbilitySystem/TinyRPGAttributeSet.h"
#include "TinyRPG/AbilitySystem/TinyRPGGameplayAbility.h"
#include "TinyRPG/ActorComponents/HealthComponent.h"
#include "TinyRPG/ActorComponents/InventoryComponent.h"
#include "TinyRPG/ActorComponents/LevelComponent.h"
#include "TinyRPG/ActorComponents/QuestLogComponent.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "TinyRPG/Actors/Weapons/Weapon.h"
#include "TinyRPG/PlayerControllers/TinyRPGPlayerController.h"

#define OUT

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	InteractionCapsule->SetupAttachment(this->GetCapsuleComponent());
	InteractionCapsule->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	InteractionCapsule->SetRelativeLocation(FVector(115.f, 0.f, 40.f));
	InteractionCapsule->SetCapsuleHalfHeight(80.f);
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	QuestLogComponent = CreateDefaultSubobject<UQuestLogComponent>(TEXT("QuestLogComponent"));
	LevelComponent = CreateDefaultSubobject<ULevelComponent>(TEXT("LevelComponent"));

	HealthComponent->SetRestoreHealth(false);
	LevelComponent->OnLevelUp.AddDynamic(HealthComponent, &UHealthComponent::RestoreFullHealth);

	AbilitySystemComponent = CreateDefaultSubobject<UTinyRPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<UTinyRPGAttributeSet>(TEXT("Attributes"));

	Tags.Add("Player");
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnOverlapEnd);
	
	if(ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController()))
	{
		PlayerController->CreateInventory();
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SpawnAndAttachWeapon();
}

void AMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		OverlappingPickUpActor = Cast<APickUpActor>(OtherActor);
	}
}

void AMainCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingPickUpActor = nullptr;
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMainCharacter::Interaction);
	// PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ATinyRPGCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AMainCharacter::ToggleInventory);
	//PlayerInputComponent->BindAction(TEXT("SelectItem"), IE_Pressed, this, &AMainCharacter::UseItem);

	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "ETinyRPGAbilityInputId", static_cast<int32>(ETinyRPGAbilityInputId::Confirm), static_cast<int32>(ETinyRPGAbilityInputId::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AMainCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AMainCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

bool AMainCharacter::GetHittedActor(FHitResult& OutHit, FVector& OutHitDirection, float Distance) const
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

void AMainCharacter::Interaction()
{
	if (OverlappingPickUpActor == nullptr || InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->AddToInventory(OverlappingPickUpActor);
}

// void AMainCharacter::Attack()
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

void AMainCharacter::ToggleInventory() 
{
	ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ToggleInventoryVisibility();
	}
}

void AMainCharacter::SpawnAndAttachWeapon()
{
	if(WeaponClass == nullptr)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);
	
	AttachWeaponToSheathSocket();
}

void AMainCharacter::AttachWeaponToHandSocket() {
	const FName RightWeaponSocketName = "RightHandWeapon";
	EquippedWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightWeaponSocketName);

	bIsWeaponSheathed = false;
}

void AMainCharacter::AttachWeaponToSheathSocket() {
	const FName SwordSheathSocket = "SwordSheath";
	EquippedWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SwordSheathSocket);

	bIsWeaponSheathed = true;
}
