// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/Save/PlayerSaveData.h"
#include "Data/Save/GameSaveData.h"
#include "TD_PlayerState.generated.h"


struct FPlayerData;
struct FBuildingTowers;

UCLASS()
class STONEDEFENCE_API ATD_PlayerState : public APlayerState
{
	GENERATED_BODY()
	ATD_PlayerState();

public:
	FPlayerData& GetPlayerData();
	FBuildingTowers& GetBuildingTower(const FGuid& ID);
	const TArray<const FGuid*> GetBuildingTowersID();
	//塔建造队列数据增删
	const FBuildingTowers& AddBuildingTower(const FGuid& ID, const FBuildingTowers& Data);
	void RequestInventorySlotSwap(const FGuid& A, const FGuid& B);
	UPlayerSaveData* GetSaveData();

protected:
	UPROPERTY()
	UPlayerSaveData* SaveData;

private:
	UFUNCTION()
	FBuildingTowers& GetBuildingTowersNULL();

	FBuildingTowers BuildingTowersNULL;
};
