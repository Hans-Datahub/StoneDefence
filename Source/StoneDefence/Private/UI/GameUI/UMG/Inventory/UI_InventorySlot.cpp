// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DragDrop/SD_DragDropOperation.h"
#include "UI/GameUI/UMG/Inventory/DragDrop/UI_IconDragDrop.h"
#include "UI/GameUI/UMG/Tip/UI_TowerTip.h"
#include "Global/UI_Data.h"

//__pragma(optimize("", off))

void UUI_InventorySlot::NativeConstruct() {
	Super::NativeConstruct();
	//ClickButton->OnClicked.AddDynamic(this, &UUI_InventorySlot::OnClickedWidget); //父类中已绑定
	if (SlotImageCD) {
		DynamicCDMaterial = SlotImageCD->GetDynamicMaterial();
	}
}

void UUI_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	//若槽内无内容，清空并跳过UpdateTowersCD()
	if (!GetBuildingTower().IsValid()) {
		ClearSlotAfterFinishedDrag();
		return;
	}
	if (!GetBuildingTower().isCDFreezed) {
		if (!GetBuildingTower().isIconDragged) {
			UpdateTowersCD(InDeltaTime);
		}
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


void UUI_InventorySlot::UpdateTowersCD(float InDeltaTime) {
	if (GetBuildingTower().TowersPrepareBuildingNumber > 0) {//若有等待建造防御塔
		if (GetBuildingTower().CurrentConstructionTowersCD > 0) {//若目前正在建造
			//内容与服务端中的逻辑判断重复，故删除，仅由服务器做逻辑判断，客户端仅做UI响应
		}
		else {//若目前建造完毕
		}
	}
}

void UUI_InventorySlot::UpdateTowersBuildingInfo() {
	UpdateSlotInfo(GetBuildingTower().TowersConstructionNumber, GetBuildingTower().CurrentConstructionTowersCD);//此处参数2是否和父类中的SlotImageCD重复了？
	DisplaySlotNumber(TPBNumber, GetBuildingTower().TowersPrepareBuildingNumber);
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

				ClearSlotAfterFinishedDrag();//隐藏自己
			}
		}
	}
}

bool UUI_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool isDrop = false;
	if (USD_DragDropOperation* SD_DragDropOperation = Cast<USD_DragDropOperation>(InOperation)) {
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(SD_DragDropOperation->Payload)) {//源槽位
			//服务器请求
			GetPlayerState()->SetTowersDragIconState(MyInventorySlot->GUID, false);
			GetPlayerState()->RequestInventorySlotSwap(GUID, MyInventorySlot->GUID);//数据互换

			UpdateUI();//目标位置			
			MyInventorySlot->UpdateUI();//原位置
			isDrop = true;
		}
	}
	return isDrop;
}

void UUI_InventorySlot::UpdateUI() {
	if (!GetBuildingTower().Icon) {//若目标槽位无值，即拖至空槽
		ClearSlotAfterFinishedDrag();
		return;
	}

	//若目标槽位有值，即交换槽位
	UpdateSlotUI(GetBuildingTower().Icon, GetBuildingTower().TowersPrepareBuildingNumber);//俩槽位的内容在OnDrop中已经互换，此处仅需赋值即可
	if (GetBuildingTower().TowersPrepareBuildingNumber > 0)
		TPBNumber->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else {//若无在建防御塔，隐藏CD背景和CD
		SlotImageCD->SetVisibility(ESlateVisibility::Hidden);
		SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
	}

	UpdateTowersBuildingInfo();//拖拽至新格子时更新参数，防止拖拽后参数为0
	DrawSlotCD(GetBuildingTower().GetTowerConstructionCDPercentage());//重新绘制CD防止原位置残留
}

void UUI_InventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	if(!bLockedGUID)
		IconGUID4DollSelection = GUID;
}

void UUI_InventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) {

}

void UUI_InventorySlot::ClearSlotAfterFinishedDrag() {
	Super::ClearSlotAfterFinishedDrag();
	TPBNumber->SetVisibility(ESlateVisibility::Hidden);
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

//__pragma(optimize("", on))