 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/Core/PlayerData.h"
#include "Data/Save/GameSaveData.h"
#include "PlayerSaveData.generated.h"


UCLASS()
class STONEDEFENCE_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
public:
	//玩家数据
	UPROPERTY(SaveGame)
	FPlayerData PlayerDatas;
	//背包数据
	UPROPERTY(SaveGame)
	TMap<FGuid, FBuildingTowers> BuildingTowers;

	void Init();
	bool IsValid();
};