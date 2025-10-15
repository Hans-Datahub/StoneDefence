// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StoneDefenceType.h"
#include "Data/SkillData.h"
#include "Data/PlayerSkillData.h"
#include "PlayerData.generated.h"

USTRUCT()
struct FPlayerData {
	GENERATED_BODY()

	FPlayerData();

	void Init();

	bool IsValid();

	UPROPERTY(SaveGame)
		int32 PlayerID;

	UPROPERTY(SaveGame)
		FName PlayerName;

	UPROPERTY(SaveGame)
		TEnumAsByte<ETeam> Team;

	UPROPERTY(SaveGame)
		FString Account;

	UPROPERTY(SaveGame)
		int32 GameGold;

	UPROPERTY(SaveGame)
		int32 Diamonds;

	UPROPERTY(SaveGame)
		int32 Coppers;

	UPROPERTY(SaveGame)
		float GameGoldTime;

	UPROPERTY(SaveGame)
		float MaxGameGoldTime;

	UPROPERTY(SaveGame)
		TArray<int32> SkillID;
};
