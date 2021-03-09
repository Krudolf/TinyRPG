// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGCharacter.h"
#include "DrawDebugHelpers.h"
#include "TinyRPG/Actors/PickUpActor.h"
#include "TinyRPG/PlayerControllers/TinyRPGPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyRPG/TinyRPGGameModeBase.h"
#include "TinyRPG/ActorComponents/InventoryComponent.h"

#define OUT

// Sets default values
ATinyRPGCharacter::ATinyRPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	InteractionCapsule->SetupAttachment(this->GetCapsuleComponent());
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void ATinyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->CreateInventory();
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATinyRPGCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ATinyRPGCharacter::OnOverlapEnd);
}

void ATinyRPGCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		OverlapingPickUpActor = Cast<APickUpActor>(OtherActor);
		ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController());
		PlayerController->CreatePickUpWidget();
	}
}

void ATinyRPGCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController());
	PlayerController->RemovePickUpWidget();
	OverlapingPickUpActor = nullptr;
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
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ATinyRPGCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Hit"), IE_Pressed, this, &ATinyRPGCharacter::Hit);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ATinyRPGCharacter::ToggleInventory);
	//PlayerInputComponent->BindAction(TEXT("SelectItem"), IE_Pressed, this, &ATinyRPGCharacter::UseItem);
}

void ATinyRPGCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ATinyRPGCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

bool ATinyRPGCharacter::GetHittedActor(FHitResult& OutHit, FVector& OutHitDirection, float Distance)
{
	AController* CharacterController = GetController();
	FVector Location;
	FRotator Rotation;
	CharacterController->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector End = Location + Rotation.Vector() * Distance;
	//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 2);

	OutHitDirection = -Rotation.Vector();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	return GetWorld()->LineTraceSingleByChannel(OUT OutHit, Location, End, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ATinyRPGCharacter::Interact()
{
	if (OverlapingPickUpActor == nullptr)
	{
		return;
	}

	if (InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->AddToInventory(OverlapingPickUpActor);
}

void ATinyRPGCharacter::Hit()
{
	FHitResult Hit;
	FVector HitDirection;
	bool bSuccess = GetHittedActor(Hit, HitDirection, HitDistance);
	if (bSuccess)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, false, 2);
		UE_LOG(LogTemp, Warning, TEXT("Hitted actor %s"), *Hit.Actor->GetName());

		AActor* ActorHitted = Hit.GetActor();
		if (ActorHitted != nullptr)
		{
			FPointDamageEvent PointDamageEvent(Damage, Hit, HitDirection, nullptr);
			ActorHitted->TakeDamage(Damage, PointDamageEvent, GetController(), this);
		}

		//TODO: Call takeDamage to damage the actor
	}
}

void ATinyRPGCharacter::ToggleInventory()
{
	ATinyRPGPlayerController* PlayerController = Cast<ATinyRPGPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->ToggleInventoryVisibility();
	}
}
