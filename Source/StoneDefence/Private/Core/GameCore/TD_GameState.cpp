 // Fill out your copyright notice in the Description page of Project Settings.

#include "Core/GameCore/TD_GameState.h"
#include "Character/Core/RuleOfCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../StoneDefenceMacro.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
//#include "Item/SpawnPoint.h"
#include "Data/Core/GameData.h"
#include "../StoneDefenceUtils.h"
#include "Character/CharacterCore/Monsters.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Data/Save/GameSaveData.h"


class ATowers;
class AMonsters;

ATD_GameState::ATD_GameState() {
	PrimaryActorTick.bCanEverTick = true;

	//角色配置表路径查找与赋值
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Towers(TEXT("/Game/GameData/TowersData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Monsters(TEXT("/Game/GameData/MonstersData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Skill(TEXT("/Game/GameData/CharacterSkillData"));

	AITowerCharacterData = MyTable_Towers.Object;
	AIMonsterCharacterData = MyTable_Monsters.Object;
	CharacterSkillData = MyTable_Skill.Object;

	//GetPlayerSaveData()->InitSaveGame(GetWorld());
}


bool ATD_GameState::SaveGameData(int32 SaveIndex) {
	if (SaveData && SlotList) {
		SlotList->SlotList.AddGameData(SaveIndex);
		return UGameplayStatics::SaveGameToSlot(SaveData, FString::Printf(TEXT("SaveSlot_%i"), SaveIndex), 0)
			&& UGameplayStatics::SaveGameToSlot(SlotList, FString::Printf(TEXT("SlotList")), 0);
	}
	return false;
}

bool ATD_GameState::ReadGameData(int32 SaveIndex) {
	SaveData = Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SaveSlot_%i"), SaveIndex), 0));
	return SaveData != NULL;
}


UGameSaveSlotList* ATD_GameState::GetGameSaveSlotList() {
	if (!SlotList) {
		SlotList = Cast<UGameSaveSlotList>(UGameplayStatics::LoadGameFromSlot(FString::Printf(TEXT("SlotList")), 0));
			if (!SlotList){
				SlotList = Cast<UGameSaveSlotList>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
			}
	}
	return SlotList;
}


UGameSaveData* ATD_GameState::GetGameSaveData() {
	if (!SaveData) {
		SaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
	}
	return SaveData;
}


FCharacterData& ATD_GameState::AddCharacterData(const FGuid& ID, const FCharacterData& Data) {
	return GetGameSaveData()->CharacterDatas.Add(ID, Data);
}
bool ATD_GameState::RemoveCharacterData(const FGuid& ID) {
	return GetGameSaveData()->CharacterDatas.Remove(ID) > 0;
}

FCharacterData& ATD_GameState::GetCharacterData(const FGuid& ID) {
	//若ID所指目标在CharacterDatas中未登记，则报错"The Current [ID] is invalid"
	if (GetGameSaveData()->CharacterDatas.Contains(ID)) {
		return GetGameSaveData()->CharacterDatas[ID];
	}
	TD_LOGPRINT(Error,"The [%i] was not found in CharacterDatas", *ID.ToString());
	return GetCharacterDataNULL();
	//return CharacterDatas.Find(Hash);

}

const TArray<FCharacterData*>& ATD_GameState::GetTowerDataFromTable() {
	if (!CacheTowerDatas.Num()) //若缓存内无数据，直接读取行
		AITowerCharacterData->GetAllRows(TEXT("Character Data"), CacheTowerDatas);
	return CacheTowerDatas;
}

const TArray<FCharacterData*>& ATD_GameState::GetMonsterDataFormTable() {
	if (!CacheMonsterDatas.Num()) //若缓存内无数据，直接读取行
		AIMonsterCharacterData->GetAllRows(TEXT("Character Data"), CacheMonsterDatas);
	return CacheMonsterDatas;
}

const TArray<FSkillData*>& ATD_GameState::GetSkillDataFromTable() {
	if (!CacheSkillDatas.Num()) //若缓存内无数据，直接读取行
		CharacterSkillData->GetAllRows(TEXT("Skill Data"), CacheSkillDatas);
	return CacheSkillDatas;
}



FGameInstanceDatas& ATD_GameState::GetGameData() {
	return GetGameSaveData()->GameDatas;
}


const FCharacterData& ATD_GameState::GetCharacterDataByID(int32 ID, ECharacterType Type) {
	auto GetMyCharacterData = [&](const TArray<FCharacterData*>& Datas, int32 ID)->const FCharacterData& {
		for (const auto& Temp : Datas) {
			if (Temp->ID == ID)
				return *Temp;
		}
		return CharacterDataNULL;
	};
	switch (Type) {
		case ECharacterType::TOWER:{
			const TArray<FCharacterData*> Datas = GetTowerDataFromTable();
			return GetMyCharacterData(Datas,ID);
		}
		case ECharacterType::MONSTER: {
			const TArray<FCharacterData*> Datas = GetMonsterDataFormTable();
			return GetMyCharacterData(Datas,ID);
		}
	}

	
	return GetCharacterDataNULL();
}

FCharacterData& ATD_GameState::GetCharacterDataNULL() {
	//此处将CharacterDataNULL换为DummyData是为了在编辑器状态下不触发解引用空指针的报错
	//因为编辑器状态下获取不到GameState，从而GetCharacterData是在解引用空指针
	//静态变量能够在整个程序生命周期内存在，故不会出现解引用空指针的情况，虽然可能没值，但一定有地址
	static FCharacterData DummyData;
	return DummyData;
}


FSkillData& ATD_GameState::AddSkillData(const FGuid& CharacterID, const FGuid& SkillID, const FSkillData& Data) {
	FCharacterData& CharacterData = GetCharacterData(CharacterID);
	if (CharacterData.IsValid()) {		
		return CharacterData.AdditionalSkillData.Add(SkillID, Data);
	}
	return SkillDataNULL;
}
FSkillData& ATD_GameState::GetSkillData(const FGuid& SkillID) {
	for (auto& Temp : GetGameSaveData()->CharacterDatas) {
		if(Temp.Value.AdditionalSkillData.Contains(SkillID)){
			return Temp.Value.AdditionalSkillData[SkillID];
		}
	}
	return SkillDataNULL;
}

const FSkillData* ATD_GameState::GetSkillData(const int32& SkillID)
{
	const TArray<FSkillData*>& SkillArray = GetSkillDataFromTable();
	for (auto& Temp : SkillArray) {
		if (Temp->SkillID == SkillID) {
			return Temp;
		}
	}
	return nullptr;
}//此处没有依照教程

FSkillData& ATD_GameState::GetSkillData(const FGuid& CharacterID, const FGuid& SkillID){
	FCharacterData& CharacterData = GetCharacterData(CharacterID);
	if (CharacterData.IsValid()) {
		if (CharacterData.AdditionalSkillData.Contains(SkillID))
			return CharacterData.AdditionalSkillData[SkillID];

	}
	return SkillDataNULL;
}

int32 ATD_GameState::RemoveSkillData(const FGuid& SkillID) {
	for (auto& Temp : GetGameSaveData()->CharacterDatas) {
		return Temp.Value.AdditionalSkillData.Remove(SkillID);
	}
	return INDEX_NONE;
}

void ATD_GameState::AddSkillDataTemplateToCharacterData(const FGuid& CharacterID, int32 SkillID) {
	if (const FSkillData* InData = GetSkillData(SkillID)) {
		for (auto& Temp : GetGameSaveData()->CharacterDatas) {
			if(CharacterID == Temp.Key) {
				Temp.Value.CharacterSkill.Add(*InData);
				Temp.Value.CharacterSkill[Temp.Value.CharacterSkill.Num() - 1].ResetCD();
				break;
			}
		}
	}
}

bool ATD_GameState::IsVerificationSkillTemplate(const FCharacterData& CharacterData, int32 SkillID) {
	for (auto& InSkill : CharacterData.CharacterSkill) {
		if (InSkill.SkillID == SkillID) {
			return true;
		}
	}
	return false;
}

bool ATD_GameState::IsVerificationSkillTemplate(const FGuid& CharacterID, int32 SkillID) {
	const FCharacterData& InData = GetCharacterData(CharacterID);
	if (InData.IsValid()) {
		return IsVerificationSkillTemplate(InData,SkillID);
	}
	return false;
}

bool ATD_GameState::IsVerificationSkill(const FCharacterData& SkillList, int32 SkillID)
{
	for (auto& AdditionalSkill : SkillList.AdditionalSkillData) {
		if (AdditionalSkill.Value.SkillID == SkillID) {
			return true;
		}
	}
	return false;
}

bool ATD_GameState::IsVerificationSkill(const FGuid& CharacterID, int32 SkillID)
{
	const FCharacterData& InData = GetCharacterData(CharacterID);
	if (InData.IsValid()) {
		return IsVerificationSkill(InData, SkillID);
	}
	return false;
}


void ATD_GameState::AddSkill(const FGuid& CharacterGUID, int32 InSkillID) {

}


void ATD_GameState::AddSkill(TPair<FGuid, FCharacterData>& SkillTakerData, FSkillData& InSkill) {
	if (!IsVerificationSkill(SkillTakerData.Value, InSkill.SkillID)) {
		FGuid TempSkillID = FGuid::NewGuid();

		SkillTakerData.Value.AdditionalSkillData.Add(TempSkillID, InSkill).ResetDuration();

		//通知代理 在UI模块显示相应技能图标	
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
			MyPlayerController->AddSkillSlot_Server(SkillTakerData.Key, TempSkillID);
		});
	}
}

void ATD_GameState::AddSkills(TArray<TPair<FGuid, FCharacterData>*>& SkillTakerDataArray, FSkillData& InSkill) {
	for (auto& Data : SkillTakerDataArray)
		AddSkill(*Data, InSkill);
}

bool ATD_GameState::SetSubmissionDataType(FGuid CharacterID, int32 SkillID, ESubmissionSkillRequestType Type) {
	FCharacterData& InCharacterData = GetCharacterData(CharacterID);
	if (InCharacterData.IsValid()) {
		for (auto& Temp : InCharacterData.CharacterSkill) {
			if (SkillID == Temp.SkillID) {
				Temp.SubmissionSkillRequestType = Type;
				return true;
			}
		}	
	}
	return false;
}