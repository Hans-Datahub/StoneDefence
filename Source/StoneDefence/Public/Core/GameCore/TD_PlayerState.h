// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/Save/PlayerSaveData.h"
#include "Data/Save/GameSaveData.h"
#include "Kismet/GameplayStatics.h"
#include "TD_PlayerState.generated.h"


struct FPlayerData;
struct FBuildingTowers;

UCLASS()
class STONEDEFENCE_API ATD_PlayerState : public APlayerState
{
	GENERATED_BODY()
	ATD_PlayerState();
	virtual void BeginPlay() override;
	bool bInitialized = false;

	//缓存池
	TArray<FPlayerSkillData*> CachePlayerSkillDatas;

public:
	FBuildingTowers& GetBuildingTower(const FGuid& ID);
	const TArray<const FGuid*> GetBuildingTowersID();
	//塔建造队列数据增删
	const FBuildingTowers& AddBuildingTower(const FGuid& ID, const FBuildingTowers& Data);
	void RequestInventorySlotSwap(const FGuid& A, const FGuid& B);
	UPlayerSaveData* GetSaveData();
	FPlayerData& GetPlayerData();
	
	FPlayerSkillData* GetPlayerSkillData(const FGuid& SkillGuid);// = GetSkillDatas()
	const TArray<const FGuid*> GetPlayerSkillDataID();// = GetSkillDatasID()

	const TArray<FPlayerSkillData*>& GetPlayerSkillDataFromTable();
	const FPlayerSkillData* GetPlayerSkillData(const int32& PlayerSkillID);
	bool SkillVerification(const FGuid& SlotID);
	void UsePlayerSkill(const FGuid& SlotID);
	void AddPlayerSkill(const FGuid* Guid, int32 SkillID);

	/*--------------------------------UI控制部分------------------------------------*/
	UFUNCTION(/*Server*/)
		void TowersPrepareBuildingNumber(const FGuid& InventoryGUID);
	UFUNCTION(/*Server*/)
		void SetTowersDragIconState(const FGuid& InventoryGUID, bool bDragIcon);
	UFUNCTION(/*Server*/)
		void TowerConstructionNumber(const FGuid& InventoryGUID, int32 InValue);

protected:
	UPROPERTY()
	UPlayerSaveData* SaveData;
	UPROPERTY()
	UDataTable* PlayerSkillData;

private:
	UFUNCTION()
	FBuildingTowers& GetBuildingTowersNULL();

	FBuildingTowers BuildingTowersNULL;

};
