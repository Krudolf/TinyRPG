// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EObjectiveType : uint8 {
	Location	UMETA(DisplayName = "Location"),
    Interact	UMETA(DisplayName = "Interact"),
	Collect		UMETA(DisplayName = "Collect"),
	Kill		UMETA(DisplayName = "Kill")
};