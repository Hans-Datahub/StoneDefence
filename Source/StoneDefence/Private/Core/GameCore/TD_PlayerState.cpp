// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_PlayerState.h"
#include "../StoneDefenceUtils.h"

ATD_PlayerState::ATD_PlayerState() {
	//��BuildingTowers����21�����󣬶�ӦUI�ϵ�21����������λ
	for (int32 i = 0; i < 21; i++)
		GetSaveData()->BuildingTowers.Add(FGuid::NewGuid(), FBuildingTowers());
}

FPlayerData& ATD_PlayerState::GetPlayerData() {
	//if (ATD_GameState* GameState = GetGameState<ATD_GameState>())
	//	TD_LOGPRINT(Error, "ATD_PlayerState::GetPlayerData() �� GetGameState Successed.");
	//else 
	//	TD_LOGPRINT(Error, "ATD_PlayerState::GetPlayerData() �� GetGameState failed.");

	return GetSaveData()->PlayerDatas;

}

FBuildingTowers& ATD_PlayerState::GetBuildingTower(const FGuid& ID) {
	//��ID��ָĿ����CharacterDatas��δ�Ǽǣ��򱨴�"The Current [ID] was not found"
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

void ATD_PlayerState::RequestInventorySlotSwap(const FGuid& A, const FGuid& B) {//AΪĿ��λ�ã�BΪԭλ��
	FBuildingTowers& ASlot = GetBuildingTower(A);
	FBuildingTowers& BSlot = GetBuildingTower(B);
	if (ASlot.IsValid()) {//��Ŀ��λ����ֵ������
		FBuildingTowers TempSlot = ASlot;
		ASlot = BSlot;
		BSlot = TempSlot;
	}
	else {//�����ƶ�
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