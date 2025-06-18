// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_Inventory.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Engine/StaticMeshActor.h"
#include "Character/CharacterCore/Towers.h"
#include "../StoneDefenceUtils.h"



void UUI_Inventory::NativeConstruct() {
	Super::NativeConstruct();

	LogoutInventorySlot(3, 7);
	
	GetPlayerController()->EventMouseMiddlePressed.BindUObject(this, &UUI_Inventory::SpawnTowerDollPressed);
	GetPlayerController()->EventMouseMiddleReleased.BindUObject(this, &UUI_Inventory::SpawnTowerDollRealeased);

}

void UUI_Inventory::UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD) {
	for (auto& Temp : InventorySlotArray) {
		if (Temp->GUID == InventorySlotGUID) {
			if (bInCD) {
				Temp->DrawTowersCD(Temp->GetBuildingTower().GetTowerConstructionCDPercentage());
			}
			else {
				Temp->DrawTowersCD(0.f);
			}

			Temp->UpdateTowersBuildingInfo();
			break;
		}
	}
}

void UUI_Inventory::LogoutInventorySlot(int32 ColumnNumber, int32 RowNumber) {
	if (InventorySlotClass) {
		//生成背包格子
		for (int32 MyRow = 0; MyRow < RowNumber; MyRow++) {
			for (int32 MyColumn = 0; MyColumn < ColumnNumber; MyColumn++) {
				UUI_InventorySlot* SlotWidget = CreateWidget<UUI_InventorySlot>(GetWorld(), InventorySlotClass);
				if (SlotWidget) {
					//添加相关UMG
					UUniformGridSlot* GridSlot = SlotArrayInventory->AddChildToUniformGrid(SlotWidget);
					if (GridSlot) {
						GridSlot->SetColumn(MyColumn);
						GridSlot->SetRow(MyRow);
						GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
						GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					}
					InventorySlotArray.Add(SlotWidget);
				}
			}
		}

		//填入防御塔ID
		const TArray<const FGuid*> ID = GetPlayerState()->GetBuildingTowersID();
		for (int32 i = 0; i < ColumnNumber * RowNumber; i++) {
			InventorySlotArray[i]->GUID = *ID[i];
		}

		//填入防御塔数据
		const TArray<FCharacterData*>& Datas = GetGameState()->GetTowerDataFromTable();
		for (int32 i = 0; i < Datas.Num(); i++) {
			InventorySlotArray[i]->GetBuildingTower().TowerID = Datas[i]->ID;
			InventorySlotArray[i]->GetBuildingTower().BuildingCost = Datas[i]->Gold;
			InventorySlotArray[i]->GetBuildingTower().MaxConstructionTowersCD = Datas[i]->CD;
			InventorySlotArray[i]->GetBuildingTower().Icon = Datas[i]->Icon.LoadSynchronous();
			InventorySlotArray[i]->UpdateUI();
		}
	}
}

FBuildingTowers& UUI_Inventory::GetBuildingTower() {
	return GetPlayerState()->GetBuildingTower(IconGUID4DollSelection);
}

void UUI_Inventory::SpawnTowerDollPressed() {
	if (GetBuildingTower().IsValid()) {
		bLockedGUID = true;
		if (GetBuildingTower().TowersConstructionNumber >= 1) {
			int32 TowersID = GetBuildingTower().TowerID;
			if (AStaticMeshActor* MeshActor = StoneDefenceUtils::SpawnTowerDoll(GetWorld(), TowersID)) {
				for (int32 i = 0; i < MeshActor->GetStaticMeshComponent()->GetNumMaterials(); i++){
					MeshActor->GetStaticMeshComponent()->SetMaterial(i, DollMaterial);
				}
				TowerDoll = MeshActor;			
			}		
		}
	}
}

void UUI_Inventory::SpawnTowerDollRealeased() {
	if (GetBuildingTower().IsValid()) {
		if (TowerDoll) {
			if (GetBuildingTower().TowersConstructionNumber >= 1) {
				if (ATowers* CharacterActor = GetPlayerController()->SpawnTower
				(GetBuildingTower().TowerID,1, TowerDoll->GetActorLocation(), TowerDoll->GetActorRotation())) {
					GetBuildingTower().TowersConstructionNumber--;
				}
			}
			TowerDoll->Destroy();
			TowerDoll = nullptr;
		}
	}
	IconGUID4DollSelection = FGuid();
	bLockedGUID = false;

}