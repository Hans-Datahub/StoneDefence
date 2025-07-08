// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "Global/UI_Data.h"
#include "UI_Inventory.generated.h"

class UUI_InventorySlot;

UCLASS()
class STONEDEFENCE_API UUI_Inventory : public UUI_RuleOfWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotArrayInventory;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_InventorySlot> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	UMaterialInterface* DollMaterial;
public:
	virtual void NativeConstruct();

	void LogoutInventorySlot(int32 ColumnNumber, int32 RowNumber);

	void SpawnTowerDollPressed();
	void SpawnTowerDollRealeased();

	FBuildingTowers& GetBuildingTower();

	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);


private:

	TArray<UUI_InventorySlot*> InventorySlotArray;
};
