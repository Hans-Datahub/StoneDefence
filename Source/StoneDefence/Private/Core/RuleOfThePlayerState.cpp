// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RuleOfThePlayerState.h"
#include "Kismet/GameplayStatics.h"

bool ARuleOfThePlayerState::ClearPlayerData(int32 SaveNumber)
{
	FString SlotName = FString::Printf(TEXT("PlayerData_%i"), SaveNumber);
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}
