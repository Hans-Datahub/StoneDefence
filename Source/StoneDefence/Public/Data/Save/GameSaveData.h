// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Core/SaveGameCore.h"
#include "Data/CharacterData.h"
#include "Data/GameData.h"
#include "Data/PlayerData.h"
#include "GameSaveData.generated.h"

/**
 * 
 */
USTRUCT()
//存储插槽中塔的相关数据
struct FBuildingTowers {
	GENERATED_BODY()

	FBuildingTowers();

	UPROPERTY(SaveGame)
	int32 TowerID;
	UPROPERTY(SaveGame)
	int32 BuildingCost;
	UPROPERTY(SaveGame)
	int32 TowersPrepareBuildingNumber;// 建造队列数量
	UPROPERTY(SaveGame)
	int32 TowersConstructionNumber;
	UPROPERTY(SaveGame)
	float MaxConstructionTowersCD;
	UPROPERTY(SaveGame)
	float CurrentConstructionTowersCD;
	UPROPERTY(SaveGame)
	UTexture2D* Icon;
	UPROPERTY(SaveGame)
	bool isIconDragged;
	UPROPERTY(SaveGame)
	bool isCDFreezed;//当游戏暂停时，CD也需要暂停
	UPROPERTY(SaveGame)
	bool CallUpdateTowrsInfoOrNot;

public:
	void Init();

	float GetTowerConstructionCDPercentage();

	bool IsValid();

	void ResetCD();
};

UCLASS()
class STONEDEFENCE_API UGameSaveData : public USaveGameCore
{
	GENERATED_BODY()


public:
	UPROPERTY(SaveGame)
		TMap<FGuid, FCharacterData> CharacterDatas;

	UPROPERTY(SaveGame)
		FGameInstanceDatas GameDatas;


	void InitSaveGame(UWorld* InWorld)override;

};