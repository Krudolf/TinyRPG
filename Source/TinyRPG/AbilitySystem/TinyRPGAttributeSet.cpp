// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyRPGAttributeSet.h"

#include "Net/UnrealNetwork.h"

UTinyRPGAttributeSet::UTinyRPGAttributeSet()
{
}

void UTinyRPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTinyRPGAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTinyRPGAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTinyRPGAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
}

void UTinyRPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTinyRPGAttributeSet, Health, OldHealth);
}

void UTinyRPGAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTinyRPGAttributeSet, Stamina, OldStamina);
}

void UTinyRPGAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTinyRPGAttributeSet, AttackPower, OldAttackPower);
}

