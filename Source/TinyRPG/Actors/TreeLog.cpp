// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeLog.h"

// Sets default values
ATreeLog::ATreeLog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATreeLog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeLog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

