// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DragDrop/SD_DragDropOperation.h"
#include "UI/GameUI/UMG/Inventory/DragDrop/UI_IconDragDrop.h"
#include "UI/GameUI/UMG/Tip/UI_TowerTip.h"
#include "Global/UI_Data.h"



void UUI_InventorySlot::NativeConstruct() {
	Super::NativeConstruct();
	TISButton->OnClicked.AddDynamic(this, &UUI_InventorySlot::OnClickedWidget);
	if (TowerCD) {
		DynamicCDMaterial = TowerCD->GetDynamicMaterial();
	}
}

void UUI_InventorySlot::OnClickedWidget() {
	if (GetBuildingTower().IsValid()) {//客户端验证 降低网络带宽

		//通知服务器对塔的数量进行增加
		GetPlayerState()->TowersPrepareBuildingNumber(GUID);
	}
}

FBuildingTowers& UUI_InventorySlot::GetBuildingTower() {
	return GetPlayerState()->GetBuildingTower(GUID);
}


void UUI_InventorySlot::UpdateUI() {
	if (GetBuildingTower().Icon) {
		TowerIcon->SetVisibility(ESlateVisibility::HitTestInvisible);//防止遮挡按钮
		TowerIcon->SetBrushFromSoftTexture(GetBuildingTower().Icon);
	}
	else 
		ClearSlotAfterFinishedDrag();

	if (GetBuildingTower().CurrentConstructionTowersCD > 0)
		TowerCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (GetBuildingTower().TowersConstructionNumber > 0)
		TCCNumber->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (GetBuildingTower().TowersPrepareBuildingNumber > 0)
		TPBNumber->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UpdateTowersBuildingInfo();//拖拽至新格子时更新参数，防止拖拽后参数为0
	DrawTowersCD(GetBuildingTower().GetTowerConstructionCDPercentage());//重新绘制CD防止原位置残留
}





void UUI_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (GetBuildingTower().IsValid()) {
		if (!GetBuildingTower().isCDFreezed) {
			if (!GetBuildingTower().isIconDragged) {
				UpdateTowersCD(InDeltaTime);
			}
		}
	}
}


void UUI_InventorySlot::UpdateTowersCD(float InDeltaTime) {
	if (GetBuildingTower().TowersPrepareBuildingNumber > 0) {
		if (GetBuildingTower().CurrentConstructionTowersCD > 0) {
			DrawTowersCD(GetBuildingTower().GetTowerConstructionCDPercentage());
			GetBuildingTower().CurrentConstructionTowersCD -= InDeltaTime;
			GetBuildingTower().CallUpdateTowrsInfoOrNot = true;
			UpdateTowersBuildingInfo();
		}
		else {
			GetBuildingTower().CallUpdateTowrsInfoOrNot = false;
			GetBuildingTower().TowersPrepareBuildingNumber--;
			GetBuildingTower().TowersConstructionNumber++;
			DrawTowersCD(0.0f);

			if (GetBuildingTower().TowersPrepareBuildingNumber > 0)
				GetBuildingTower().ResetCD();
			UpdateTowersBuildingInfo();
		}
	}
}

void UUI_InventorySlot::DrawTowersCD(float InTowersCD) {
	if (DynamicCDMaterial) {
		if (InTowersCD > 0.0f && InTowersCD < 1.0f) {
			DynamicCDMaterial->SetScalarParameterValue(TowersCDClearName, true);
			TowerCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			DynamicCDMaterial->SetScalarParameterValue(TowersCDClearName, false);
			TowerCD->SetVisibility(ESlateVisibility::Hidden);
		}
		DynamicCDMaterial->SetScalarParameterValue(TowersCDMaterialName, InTowersCD);
	}
}

void UUI_InventorySlot::DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber) {
	if (TextNumber < 1 || !GetBuildingTower().IsValid()) 
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	else {
		TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TextNumber)));
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_InventorySlot::UpdateTowersBuildingInfo() {
	DisplayNumber(TowersCDValue, GetBuildingTower().CurrentConstructionTowersCD);
	DisplayNumber(TCCNumber,GetBuildingTower().TowersConstructionNumber);
	DisplayNumber(TPBNumber, GetBuildingTower().TowersPrepareBuildingNumber);
}






FReply UUI_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent()) {
		FReply Reply = FReply::Handled();
		//获取拖拽的缓存
		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid()) {
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
			return Reply;
		}
	}
	return FReply::Handled();
}

void UUI_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) {
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (GetBuildingTower().IsValid() && IconDragDropClass) {
		if (UUI_IconDragDrop* IconDragDrop = CreateWidget<UUI_IconDragDrop>(GetWorld(), IconDragDropClass)) {
			if (USD_DragDropOperation* SD_DragDropOperation =
				NewObject<USD_DragDropOperation>(GetTransientPackage(), USD_DragDropOperation::StaticClass())) {

				SD_DragDropOperation->SetFlags(RF_StrongRefOnFrame);//此处“强引用”目的是通知编辑器不要过早回收
				IconDragDrop->SetDragIcon(GetBuildingTower().Icon);
				SD_DragDropOperation->DefaultDragVisual = IconDragDrop;//拖拽时在鼠标生成同样图标
				SD_DragDropOperation->Payload = this;
				OutOperation = SD_DragDropOperation;

				GetPlayerState()->SetTowersDragIconState(GUID,true);
			}
		}
	}

}

bool UUI_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool isDrop = false;
	if (USD_DragDropOperation* SD_DragDropOperation = Cast<USD_DragDropOperation>(InOperation)) {
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(SD_DragDropOperation->Payload)) {
			//服务器请求
			GetPlayerState()->SetTowersDragIconState(MyInventorySlot->GUID, false);
			GetPlayerState()->RequestInventorySlotSwap(GUID, MyInventorySlot->GUID);

			UpdateUI();//目标位置
			MyInventorySlot->UpdateUI();//原位置
			isDrop = true;
		}
	}
	return isDrop;
}

void UUI_InventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	if(!bLockedGUID)
		IconGUID4DollSelection = GUID;
}

void UUI_InventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) {

}


void UUI_InventorySlot::ClearSlotAfterFinishedDrag() {
	TowerIcon->SetVisibility(ESlateVisibility::Hidden);
	TowerCD->SetVisibility(ESlateVisibility::Hidden);
	TPBNumber->SetVisibility(ESlateVisibility::Hidden);
	TCCNumber->SetVisibility(ESlateVisibility::Hidden);
	TowersCDValue->SetVisibility(ESlateVisibility::Hidden);
}


UWidget* UUI_InventorySlot::GetTowerTip() {
	if(TowerTipClass) {
		if (UUI_TowerTip* TowerTip = CreateWidget<UUI_TowerTip>(GetWorld(), TowerTipClass)) {
			const FCharacterData& TowerDataInfo = GetGameState()->GetCharacterDataByID(GetBuildingTower().TowerID, ECharacterType::TOWER);
			if (TowerDataInfo.IsValid()) {
				TowerTip->InitTip(TowerDataInfo);
				return TowerTip;
			}
		}	
	}
	return nullptr;
}

