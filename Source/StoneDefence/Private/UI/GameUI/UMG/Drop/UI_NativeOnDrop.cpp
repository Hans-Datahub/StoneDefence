// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
//#include "UI/GameUI/UMG/Inventory/DragDrop/UI_IconDragDrop.h"
#include "DragDrop/SD_DragDropOperation.h"

bool UUI_NativeOnDrop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool isDrop = false;
	if (USD_DragDropOperation* SD_DragDropOperation = Cast<USD_DragDropOperation>(InOperation)) {
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(SD_DragDropOperation->Payload)) {
			MyInventorySlot->GetBuildingTower().isIconDragged = false;
			MyInventorySlot->UpdateUI();
			isDrop = true;
		}
	}
	return isDrop;
}