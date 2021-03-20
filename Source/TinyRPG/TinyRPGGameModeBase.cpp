// Copyright Epic Games, Inc. All Rights Reserved.


#include "TinyRPGGameModeBase.h"

void ATinyRPGGameModeBase::RefreshQuestsVisibility()
{
	for(AQuestBase* Quest : QuestsInWorld)
	{
		if(Quest->PreRequisiteQuest != nullptr && Quest->PreRequisiteQuest->bIsCompleted && !Quest->bIsCompleted)
		{
			Quest->SetQuestVisibility(true);
		}
	}
}
