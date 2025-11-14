// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/RuleOfTheGameState.h"
#include "Data/CharacterData.h"
#include "Data/Save/GameSaveSlotList.h"
#include "Data/Save/GameSaveData.h"
#include "Engine/StaticMeshActor.h"
#include "Data/GameData.h"
#include "Data/PlayerData.h"
#include "Data/Save/PlayerSaveData.h"
#include "Data/SkillData.h"
#include "Interface/SimpleArchivesInterface.h"
#include "TD_GameState.generated.h"


class ARuleOfCharacter;
class UDataTable;
class UGameSaveData;
class UGameSaveSlotList;
struct FBuildingTowers;

UCLASS()
class STONEDEFENCE_API ATD_GameState : public ARuleOfTheGameState
{
	GENERATED_BODY()

	friend class ATD_GameMode;

public:
	UPROPERTY()
	UDataTable* AITowerCharacterData;
	UPROPERTY()
	UDataTable* AIMonsterCharacterData;
	UPROPERTY()
	UDataTable* CharacterSkillData;

	UPROPERTY()
	UGameSaveData* SaveData;


	//缓存池
	TArray<FCharacterData*> CacheTowerDatas;
	TArray<FCharacterData*> CacheMonsterDatas;
	TArray<FSkillData*> CacheSkillDatas;


public:
	ATD_GameState();

	virtual bool SaveGameData(int32 SaveIndex);
	virtual bool ReadGameData(int32 SaveIndex);

	//角色数据增删
	FCharacterData& AddCharacterData(const FGuid& ID, const FCharacterData &Data);
	bool RemoveCharacterData(const FGuid& ID);

	/*---------------------------------角色技能增删查改------------------------------------*/
	FSkillData& AddSkillData(const FGuid& CharacterID, const FGuid& SkillID, const FSkillData& Data);
	FSkillData& GetSkillData(const FGuid& SkillID);
	const FSkillData* GetSkillData(const int32& SkillID);
	FSkillData& GetSkillData(const FGuid& CharacterID, const FGuid& SkillID);
	int32 RemoveSkillData(const FGuid& SkillID);

	UFUNCTION(/*Server*/)
	void AddSkillDataTemplateToCharacterData(const FGuid& CharacterID, int32 SkillID);

	bool IsVerificationSkillTemplate(const FCharacterData& CharacterData, int32 SkillID);
	bool IsVerificationSkillTemplate(const FGuid& CharacterID, int32 SkillID);
	bool IsVerificationSkill(const FCharacterData& SkillList, int32 SkillID);
	bool IsVerificationSkill(const FGuid& CharacterID, int32 SkillID);

	void AddSkill(const FGuid& CharacterGUID, int32 InSkillID);
	void AddSkill(TPair<FGuid, FCharacterData>& SkillTakerData, FSkillData& InSkill);
	void AddSkills(TArray<TPair<FGuid, FCharacterData>*>& SkillTakerDataArray, FSkillData& InSkill);

	bool SetSubmissionDataType(FGuid CharacterID, int32 SkillID, ESubmissionSkillRequestType Type);

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

	//UGameSaveData* GetGameSaveData();

	UGameSaveData* GetSaveData();


protected:
	//UGameSaveSlotList* GetGameSaveSlotList();

private:
	FCharacterData CharacterDataNULL;
	FSkillData SkillDataNULL;

};