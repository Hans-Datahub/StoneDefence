// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Core/CharacterData.h"
#include "Data/Save/GameSaveSlotList.h"
#include "Data/Save/GameSaveData.h"
#include "Engine/StaticMeshActor.h"
#include "Data/Core/GameData.h"
#include "Data/Core/PlayerData.h"
#include "Data/Core/SkillData.h"
#include "TD_GameState.generated.h"


class ARuleOfCharacter;
class UDataTable;
class UGameSaveData;
class UGameSaveSlotList;
struct FBuildingTowers;

UCLASS()
class STONEDEFENCE_API ATD_GameState : public AGameState
{
	GENERATED_BODY()

	friend class ATD_GameMode;

	UPROPERTY()
	UDataTable* AITowerCharacterData;
	UPROPERTY()
	UDataTable* AIMonsterCharacterData;
	UPROPERTY()
	UDataTable* CharacterSkillData;

	UPROPERTY()
	UGameSaveData* SaveData;
	UPROPERTY()
	UGameSaveSlotList* SlotList;

	//缓存池
	TArray<FCharacterData*> CacheTowerDatas;
	TArray<FCharacterData*> CacheMonsterDatas;
	TArray<FSkillData*> CacheSkillDatas;


public:
	ATD_GameState();

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveGameData(int32 SaveIndex);

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool ReadGameData(int32 SaveIndex);

	//角色数据增删
	FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData &Data);
	bool RemoveCharacterData(const FGuid& ID);

	/*---------------------------------角色技能增删查改------------------------------------*/
	FSkillData& AddSkillData(const FGuid& CharacterID, const FGuid& SkillID, const FSkillData& Data);
	FSkillData& GetSkillData(const FGuid& SkillID);
	FSkillData& GetSkillData(const FGuid& CharacterID, const FGuid& SkillID);
	int32 RemoveSkillData(const FGuid& SkillID);

	void InitSkill(FCharacterData& InCharacterData);

	/*--------------------------------数据获取------------------------------------*/
	FCharacterData& GetCharacterData(const FGuid& ID);


	//bool GetTowerDataFromTable(TArray<const FCharacterData*>& Datas);
	const TArray<FCharacterData*>& GetTowerDataFromTable();
	//bool GetMonsterDataFormTable(TArray<const FCharacterData*>& Datas);
	const TArray<FCharacterData*>& GetMonsterDataFormTable();
	//模板技能
	const TArray<FSkillData*>& GetSkillDataFromTable();

	const FCharacterData& GetCharacterDataByID(int32 ID, ECharacterType Type);

	FGameInstanceDatas& GetGameData();
	

	FCharacterData& GetCharacterDataNULL();

	UGameSaveData* GetSaveData();
	/*--------------------------------------------------------------------------*/


protected:
	UGameSaveSlotList* GetGameSaveSlotList();

private:
	FCharacterData CharacterDataNULL;
	FSkillData SkillDataNULL;

};