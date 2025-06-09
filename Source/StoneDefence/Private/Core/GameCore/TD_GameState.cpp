 // Fill out your copyright notice in the Description page of Project Settings.

#include "Core/GameCore/TD_GameState.h"
#include "Character/Core/RuleOfCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../StoneDefenceMacro.h"
#include "Engine/DataTable.h"
#include "Data/Save/GameSaveData.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
//#include "Item/SpawnPoint.h"
#include "Data/Core/GameData.h"
//#include "../StoneDefenceUtils.h"
#include "Character/CharacterCore/Monsters.h"


class ATowers;
class AMonsters;

ATD_GameState::ATD_GameState() {
	PrimaryActorTick.bCanEverTick = true;

	//��ɫ���ñ�·�������븳ֵ
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Towers(TEXT("/Game/GameData/TowersData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Monsters(TEXT("/Game/GameData/MonstersData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Skill(TEXT("/Game/GameData/CharacterSkillData"));

	AITowerCharacterData = MyTable_Towers.Object;
	AIMonsterCharacterData = MyTable_Monsters.Object;
	CharacterSkillData = MyTable_Skill.Object;
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


UGameSaveData* ATD_GameState::GetSaveData() {
	if (!SaveData) {
		SaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
	}
	return SaveData;
}


FCharacterData& ATD_GameState::AddCharacterData(const FGuid& ID, const FCharacterData& Data) {
	return GetSaveData()->CharacterDatas.Add(ID, Data);
}
bool ATD_GameState::RemoveCharacterData(const FGuid& ID) {
	return GetSaveData()->CharacterDatas.Remove(ID) > 0;
}

FCharacterData& ATD_GameState::GetCharacterData(const FGuid& ID) {
	//��ID��ָĿ����CharacterDatas��δ�Ǽǣ��򱨴�"The Current [ID] is invalid"
	if (GetSaveData()->CharacterDatas.Contains(ID)) {
		return GetSaveData()->CharacterDatas[ID];
	}
	TD_LOGPRINT(Error,"The [%i] was not found in CharacterDatas", *ID.ToString());
	return GetCharacterDataNULL();
	//return CharacterDatas.Find(Hash);

}

const TArray<FCharacterData*>& ATD_GameState::GetTowerDataFromTable() {
	if (!CacheTowerDatas.Num()) //�������������ݣ�ֱ�Ӷ�ȡ��
		AITowerCharacterData->GetAllRows(TEXT("Character Data"), CacheTowerDatas);
	return CacheTowerDatas;
}

const TArray<FCharacterData*>& ATD_GameState::GetMonsterDataFormTable() {
	if (!CacheMonsterDatas.Num()) //�������������ݣ�ֱ�Ӷ�ȡ��
		AIMonsterCharacterData->GetAllRows(TEXT("Character Data"), CacheMonsterDatas);
	return CacheMonsterDatas;
}

const TArray<FSkillData*>& ATD_GameState::GetSkillDataFromTable() {
	if (!CacheSkillDatas.Num()) //�������������ݣ�ֱ�Ӷ�ȡ��
		CharacterSkillData->GetAllRows(TEXT("Skill Data"), CacheSkillDatas);
	return CacheSkillDatas;
}



FGameInstanceDatas& ATD_GameState::GetGameData() {
	return GetSaveData()->GameDatas;
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
	//�˴���CharacterDataNULL��ΪDummyData��Ϊ���ڱ༭��״̬�²����������ÿ�ָ��ı���
	//��Ϊ�༭��״̬�»�ȡ����GameState���Ӷ�GetCharacterData���ڽ����ÿ�ָ��
	//��̬�����ܹ��������������������ڴ��ڣ��ʲ�����ֽ����ÿ�ָ����������Ȼ����ûֵ����һ���е�ַ
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
	for (auto& Temp : GetSaveData()->CharacterDatas) {
		for (auto& SkillTemp: Temp.Value.AdditionalSkillData) {
			if (SkillTemp.Key == SkillID) {
				return SkillTemp.Value;
			}
		}
	}
	return SkillDataNULL;
}
FSkillData& ATD_GameState::GetSkillData(const FGuid& CharacterID, const FGuid& SkillID){
	FCharacterData& CharacterData = GetCharacterData(CharacterID);
	if (CharacterData.IsValid()) {
		for (auto& Temp : CharacterData.AdditionalSkillData) {
			if (Temp.Key == SkillID) {
				return Temp.Value;
			}
		}
	}
	return SkillDataNULL;
}

int32 ATD_GameState::RemoveSkillData(const FGuid& SkillID) {
	for (auto& Temp : GetSaveData()->CharacterDatas) {
		return Temp.Value.AdditionalSkillData.Remove(SkillID);
	}
	return INDEX_NONE;
}

void ATD_GameState::InitSkill(FCharacterData& InCharacterData) {
	const TArray<FSkillData*>& InSkillData = GetSkillDataFromTable();
	for (auto& Temp : InCharacterData.CharacterSkillID) {
		for (const FSkillData* NewSkill : InSkillData) {
			if (NewSkill->SkillID == Temp) {
				InCharacterData.CharacterSkill.Add(*NewSkill); 
				break;
			}
		}
	}
}









