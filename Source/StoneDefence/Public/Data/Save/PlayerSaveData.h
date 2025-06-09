 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveData.generated.h"

struct FBuildingTowers;
struct FPlayerData;

UCLASS()
class STONEDEFENCE_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
public:
	//�������
	UPROPERTY(SaveGame)
	FPlayerData PlayerDatas;
	//��������
	UPROPERTY(SaveGame)
	TMap<FGuid, FBuildingTowers> BuildingTowers;

	void Init();
	bool IsValid();
};