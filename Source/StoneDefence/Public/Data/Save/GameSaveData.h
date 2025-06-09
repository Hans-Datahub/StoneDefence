// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/Core/CharacterData.h"
#include "Data/Core/GameData.h"
#include "Data/Core/PlayerData.h"
#include "GameSaveData.generated.h"

/**
 * 
 */
USTRUCT()
//�洢����������������
struct FBuildingTowers {
	GENERATED_BODY()

	FBuildingTowers();

	UPROPERTY(SaveGame)
	int32 TowerID;
	UPROPERTY(SaveGame)
	int32 BuildingCost;
	UPROPERTY(SaveGame)
	int32 TowersPrepareBuildingNumber;// �����������
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
	bool isCDFreezed;//����Ϸ��ͣʱ��CDҲ��Ҫ��ͣ
	UPROPERTY(SaveGame)
	bool CallUpdateTowrsInfoOrNot;

public:
	void Init();

	float GetTowerConstructionCDPercentage();

	bool IsValid();

	void ResetCD();
};

UCLASS()
class STONEDEFENCE_API UGameSaveData : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY(SaveGame)
		TMap<FGuid, FCharacterData> CharacterDatas;

	UPROPERTY(SaveGame)
		FGameInstanceDatas GameDatas;

};