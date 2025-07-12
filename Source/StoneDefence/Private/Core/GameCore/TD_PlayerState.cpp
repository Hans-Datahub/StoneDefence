// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_PlayerState.h"

#include "../StoneDefenceUtils.h"
#include "../StoneDefenceMacro.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/GameCore/TD_PlayerController.h"


//__pragma(optimize("", off))

ATD_PlayerState::ATD_PlayerState() {
	//添加防御塔槽位和技能原本放在构造中，在技能添加进来之后，报错读取位置 0xFFFFFFFFFFFFFFFF 时发生访问冲突。故放至BeginPlay中
	//12.12开头有提及，PlayerSkillData继承自DataTable，若在构造中初始化则会报上面的错误

	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Skill(TEXT("/Game/GameData/PlayerSkillDataTable"));
	PlayerSkillData = MyTable_Skill.Object;
}

void ATD_PlayerState::BeginPlay() {
	//给BuildingTowers加入21个对象，对应UI上的21个防御塔槽位
	for (int32 i = 0; i < 21; i++)
		GetSaveData()->BuildingTowers.Add(FGuid::NewGuid(), FBuildingTowers());

	//给PlayerSkillDatas加入10个对象，对应10个技能
	for (int32 i = 0; i < 10; i++)
		GetSaveData()->PlayerSkillDatas.Add(FGuid::NewGuid(), FPlayerSkillData());

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

void ATD_PlayerState::TowersPrepareBuildingNumber(const FGuid& InventoryGUID) {
	FBuildingTowers& BT = GetBuildingTower(InventoryGUID);
	if (BT.IsValid()) {//服务器验证 防止作弊
		if (BT.BuildingCost <= GetPlayerData().GameGold) {
			BT.TowersPrepareBuildingNumber++;
			GetPlayerData().GameGold -= BT.BuildingCost;

			if (BT.CurrentConstructionTowersCD <= 0) {
				BT.ResetCD();
			}
		}
	}
}

void ATD_PlayerState::SetTowersDragIconState(const FGuid& InventoryGUID, bool bDragIcon) {
	FBuildingTowers& BT = GetBuildingTower(InventoryGUID);
	BT.isIconDragged = bDragIcon;
}

void ATD_PlayerState::TowerConstructionNumber(const FGuid& InventoryGUID, int32 InValue = INDEX_NONE) {
	FBuildingTowers& BT = GetBuildingTower(InventoryGUID);
	if (BT.IsValid()) {
		BT.TowersConstructionNumber += InValue;
	}
}



FPlayerSkillData* ATD_PlayerState::GetPlayerSkillData(const FGuid& SkillGuid) {
	//若ID所指目标在PlayerSkillDatas中未登记，则报错"The Current [ID] was not found"
	if (GetSaveData()->PlayerSkillDatas.Contains(SkillGuid)) {
		return &GetSaveData()->PlayerSkillDatas[SkillGuid];
	}
	TD_LOGPRINT(Error, "The [%i] was not found in PlayerSkillDatas", *SkillGuid.ToString());
	return nullptr;
}

const TArray<const FGuid*> ATD_PlayerState::GetPlayerSkillDataID() {
	TArray<const FGuid*> SkillIDs;
	for (const auto& Temp : GetSaveData()->PlayerSkillDatas) {
		SkillIDs.Add(&Temp.Key);
	}
	return SkillIDs;
}

const TArray<FPlayerSkillData*>& ATD_PlayerState::GetPlayerSkillDataFromTable() {
	if (!CachePlayerSkillDatas.Num()) //若缓存内无数据，直接读取行
		PlayerSkillData->GetAllRows(TEXT("PlayerSkill Data"), CachePlayerSkillDatas);
	return CachePlayerSkillDatas;
}

const FPlayerSkillData* ATD_PlayerState::GetPlayerSkillData(const int32& PlayerSkillID) {
	const TArray<FPlayerSkillData*>& InSkillData = GetPlayerSkillDataFromTable();
	for (auto& Temp : InSkillData) {
		if (Temp->SkillID == PlayerSkillID) {
			return Temp;
		}
	}
	return NULL;
}

bool ATD_PlayerState::SkillVerification(const FGuid& SlotID) {//验证成功，表明可以使用该技能
	if (FPlayerSkillData* InData = GetPlayerSkillData(SlotID)) {
		if (InData->IsValid() && InData->SkillNumber > 0 && InData->GetSkillCDPercentage() <= 0.f) {//不太理解此处第二个条件，数据有意义不就是技能个数 > 0吗？
			return true;
		}
	}
	return false;
}

void ATD_PlayerState::UsePlayerSkill(const FGuid& SlotID) {
	if(FPlayerSkillData* InData = GetPlayerSkillData(SlotID)){
		if (InData->IsValid()) {
			InData->SkillNumber--;
			InData->ResetCD();
			//通知代理 在UI模块显示相应技能图标	
			StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
				MyPlayerController->SpawnPlayerSkill_Client(InData->SkillID);
			});

		}
	}
}

void ATD_PlayerState::AddPlayerSkill(const FGuid* Guid, int32 SkillID) {
	if (const FPlayerSkillData* SkillData = GetPlayerSkillData(SkillID)) {
		GetSaveData()->PlayerSkillDatas[*Guid] = *SkillData;
	
		//通知代理 在UI模块显示相应技能图标	
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
			MyPlayerController->UpdatePlayerSkill_Client(*Guid, false);
		});
	}
}



//__pragma(optimize("", on))