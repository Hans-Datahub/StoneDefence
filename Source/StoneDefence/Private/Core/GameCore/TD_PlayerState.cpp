// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_PlayerState.h"
#include "../StoneDefenceUtils.h"
#include "../StoneDefenceMacro.h"

ATD_PlayerState::ATD_PlayerState() {
	//给BuildingTowers加入21个对象，对应UI上的21个防御塔槽位
	for (int32 i = 0; i < 21; i++)
		GetSaveData()->BuildingTowers.Add(FGuid::NewGuid(), FBuildingTowers());
}

FPlayerData& ATD_PlayerState::GetPlayerData() {
	//if (ATD_GameState* GameState = GetGameState<ATD_GameState>())
	//	TD_LOGPRINT(Error, "ATD_PlayerState::GetPlayerData() ： GetGameState Successed.");
	//else 
	//	TD_LOGPRINT(Error, "ATD_PlayerState::GetPlayerData() ： GetGameState failed.");

	return GetSaveData()->PlayerDatas;

}

FBuildingTowers& ATD_PlayerState::GetBuildingTower(const FGuid& ID) {
	//若ID所指目标在CharacterDatas中未登记，则报错"The Current [ID] was not found"
	if (GetSaveData()->BuildingTowers.Contains(ID)) {//BuildingTowers
		return GetSaveData()->BuildingTowers[ID];
	}
	TD_LOGPRINT(Error, "The [%i] was not found in BuildingTowers", *ID.ToString());
	return GetBuildingTowersNULL();
}

FBuildingTowers& ATD_PlayerState::GetBuildingTowersNULL() {
	return BuildingTowersNULL;
}

const TArray<const FGuid*> ATD_PlayerState::GetBuildingTowersID() {
	TArray<const FGuid*> TowersID;
	for (const auto& Temp : GetSaveData()->BuildingTowers) {
		TowersID.Add(&Temp.Key);
	}
	return TowersID;
}

const FBuildingTowers& ATD_PlayerState::AddBuildingTower(const FGuid& ID, const FBuildingTowers& Data) {
	return GetSaveData()->BuildingTowers.Add(ID, Data);
}

void ATD_PlayerState::RequestInventorySlotSwap(const FGuid& A, const FGuid& B) {//A为目标位置，B为原位置
	FBuildingTowers& ASlot = GetBuildingTower(A);
	FBuildingTowers& BSlot = GetBuildingTower(B);
	if (ASlot.IsValid()) {//若目标位置有值，交换
		FBuildingTowers TempSlot = ASlot;
		ASlot = BSlot;
		BSlot = TempSlot;
	}
	else {//否则移动
		ASlot = BSlot;
		BSlot.Init();
	}

}

UPlayerSaveData* ATD_PlayerState::GetSaveData() {
	if (!SaveData) {
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
	}
	return SaveData;
}