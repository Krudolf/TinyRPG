// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "TinyRPG/Characters/TinyRPGCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponStaticMesh->SetupAttachment(RootComponent);

	Tags.Add("Weapon");
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ATinyRPGCharacter>(GetOwner());
	// WeaponStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::CheckOverlappingActors()
{
	TArray<AActor*> OverlappingPawns;
	// WeaponStaticMesh->GetOverlappingActors(OverlappingPawns, APawn::StaticClass());
	WeaponStaticMesh->GetOverlappingActors(OverlappingPawns);
	
	for(AActor* Actor : OverlappingPawns)
	{
		if(Actor != GetOwner())
		{
			Actor->TakeDamage(OwnerCharacter->GetDamage(), FDamageEvent(), OwnerCharacter->GetController(), this);
		}
	}
}

