// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "UI/GameUI/UMG/Tip/UI_TowerTip.h"
#include "UI/GameUI/UMG/UI_GameMenuSystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Components/Image.h"
#include "UI/GameUI/UMG/UI_RucksackSystem.h"
#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "SimplePopupUtils.h"
#include "UI_ArchivesSystem.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "../../../../StoneDefenceUtils.h"
#include "UI_GameSettingsSystem.h"
#include "ArchiveDelegate.h"
#include "SettingDelegate.h"
#include "UI/GameUI/UMG/Inventory/UI_Inventory.h"
#include "UI/GameUI/UMG/Inventory/UI_InventorySlot.h"
#include "Data/Save/GameSaveData.h"
#include "Core/GameCore/TD_GameInstance.h"

class USD_DragDropOperation;
struct FBuildingTowers;
class ATD_GameState;


void UUI_MainScreen::NativeConstruct() {
	Super::NativeConstruct();

	GameMenuSystem->SetVisibility(ESlateVisibility::Hidden);


	//设置GameMenuSystem中的按钮绑定
	SettingsButton->OnClicked.AddDynamic(this, &UUI_MainScreen::Settings);
	SettingsButton->OnClicked.AddDynamic(this, &UUI_MainScreen::FreezeTimeAndCD);

	{
		FOnButtonClickedEvent Delegate;
		Delegate.AddDynamic(this, &UUI_MainScreen::SaveGame);
		GameMenuSystem->BindSaveGame(Delegate);
	}
	{
		FOnButtonClickedEvent Delegate;
		Delegate.AddDynamic(this, &UUI_MainScreen::SaveSettings);
		GameMenuSystem->BindSaveSettings(Delegate);
	}
	{
		FOnButtonClickedEvent Delegate;
		Delegate.AddDynamic(this, &UUI_MainScreen::ReturnGame);
		Delegate.AddDynamic(this, &UUI_MainScreen::UnFreezeTimeAndCD);
		GameMenuSystem->BindReturnGame(Delegate);
	}
	//订阅与存档系统的通信，用于弹窗关闭
	SubscribeToCloseEvent();
}

void UUI_MainScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	//显示角色信息
	if (ARuleOfCharacter* InCharacter = Cast<ARuleOfCharacter>(GetPlayerController()->GetHitResult().GetActor())) {
		const FCharacterData& CharacterData = GetGameState()->GetCharacterData(InCharacter->GUID);
		if (CharacterData.IsValid()) {
			CharacterTip->InitTip(CharacterData);
			CharacterTip->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(CharacterTip->Slot)) {
				FVector2D ScreenLocation = FVector2D::ZeroVector;
				UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetPlayerController(), GetPlayerController()->GetHitResult().Location, ScreenLocation, true);
				NewPanelSlot->SetPosition(ScreenLocation);
			}
			else {
				CharacterTip->SetVisibility(ESlateVisibility::Hidden);
			}

		}
		else {
			CharacterTip->SetVisibility(ESlateVisibility::Hidden);
		}

	}
	else {
		CharacterTip->SetVisibility(ESlateVisibility::Hidden);
	}


	//显示集火对象
	if (ClickedMonster && ClickedMonster->IsActive()) {
		FireConcentrationPoint->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(FireConcentrationPoint->Slot)) {
			FVector2D ScreenLocation = FVector2D::ZeroVector;
			UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetPlayerController(), ClickedMonster->GetActorLocation(), ScreenLocation, true);
			NewPanelSlot->SetPosition(ScreenLocation);
		}
	}
	else {
		ClickedMonster = nullptr;
		FireConcentrationPoint->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_MainScreen::UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD) {
	if (RucksackSystem) {
		RucksackSystem->UpdateInventorySlot(InventorySlotGUID, bInCD);
	}
}

void UUI_MainScreen::UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD) {
	if (PlayerSkillSystem) {
		PlayerSkillSystem->UpdatePlayerSkillSlot(PlayerSkillSlotGUID, bInCD);
	}
}

void UUI_MainScreen::Settings()
{
	if (GameMenuSystem->GetVisibility() == ESlateVisibility::Hidden)
	{
		NewWindows->SetVisibility(ESlateVisibility::Visible);
		GameMenuSystem->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		NewWindows->SetVisibility(ESlateVisibility::Hidden);
		GameMenuSystem->SetVisibility(ESlateVisibility::Hidden);
	}
}




void UUI_MainScreen::ShowGameMenu()
{
	GameMenuSystem->SetVisibility(ESlateVisibility::Visible);

}

void UUI_MainScreen::HideGameMenu()
{
	GameMenuSystem->SetVisibility(ESlateVisibility::Hidden);
}






				//------------------------GameMenuSystem中按钮的绑定函数---------------------------//

void UUI_MainScreen::SaveGame()
{
	HideGameMenu();
	if (UUI_ArchivesSystem* TempArchivesSystem = StoneDefenceUtils::CreateAssistWidget<UUI_MainScreen, UUI_ArchivesSystem>(this, ArchivesSystemClass, BoxList))
	{
		TempArchivesSystem->BindWindows(
			[&](FSimpleDelegate InDelegate)
			{
				SimplePopupUtils::CreatePopup(
					GetWorld(),
					PopupClass,
					NSLOCTEXT("UUI_MainScreen", "DeleteSaveSlot", "Are you sure you want to delete this archive ?"),
					ESimplePopupType::TWO,
					10.f,
					InDelegate);
			});
	}
}

void UUI_MainScreen::SaveSettings()
{
	HideGameMenu();
	StoneDefenceUtils::CreateAssistWidget<UUI_MainScreen, UUI_GameSettingsSystem>(this, GameSettingsClass, BoxList);
}

void UUI_MainScreen::ReturnGame()
{
	Settings();
}

void UUI_MainScreen::FreezeTimeAndCD() {
	//Freeze CD
	TArray<UUI_InventorySlot*> InventorySlotFreezeArray = RucksackSystem->Inventory->InventorySlotArray;
	for (auto& Temp : InventorySlotFreezeArray) {
		FBuildingTowers& BuildingTowers = Temp->GetBuildingTower();
		BuildingTowers.isCDFreezed = true;
	}

	// 玩家技能Freeze  //

	//Freeze Time
	GetWorld()->GetGameState<ATD_GameState>()->GetGameData().bTimeFreezed = true;
}

void UUI_MainScreen::UnFreezeTimeAndCD() {
	//UnFreeze CD
	TArray<UUI_InventorySlot*> InventorySlotFreezeArray = RucksackSystem->Inventory->InventorySlotArray;
	for (auto& Temp : InventorySlotFreezeArray) {
		FBuildingTowers& BuildingTowers = Temp->GetBuildingTower();
		BuildingTowers.isCDFreezed = false;
	}


	// 玩家技能UnFreeze  //


	//UnFreeze Time
	GetWorld()->GetGameState<ATD_GameState>()->GetGameData().bTimeFreezed = false;
}




						//--------------------------------代理部分--------------------------------//

void UUI_MainScreen::SubscribeToCloseEvent()
{
	{
		// 订阅者行为：注册监听事件
		UArchiveDelegate* ArchivesDelegates = UArchiveDelegate::GetInstance();
		if (ArchivesDelegates)
		{
			ArchivesDelegates->OnArchivesClosed.AddDynamic(this, &UUI_MainScreen::OnArchivesOrSettingsClosedHandler);
			ArchivesDelegates->OnArchiveIndexChanged.AddDynamic(this, &UUI_MainScreen::OnArchiveIndexChangedHandler);

		}
	}

	{
		// 订阅者行为：注册监听事件
		USettingDelegate* SettingsDelegates = USettingDelegate::GetInstance();
		if (SettingsDelegates)
		{
			// 关键代码：订阅者将自己的处理函数绑定到代理
			SettingsDelegates->OnSettingsClosed.AddDynamic(this, &UUI_MainScreen::OnArchivesOrSettingsClosedHandler);
		}
	}
}

void UUI_MainScreen::UnsubscribeFromCloseEvent()
{
	{
		// 订阅者行为：取消监听（防止内存泄漏）
		UArchiveDelegate* ArchivesDelegates = UArchiveDelegate::GetInstance();
		if (ArchivesDelegates)
		{
			ArchivesDelegates->OnArchivesClosed.RemoveDynamic(this, &UUI_MainScreen::OnArchivesOrSettingsClosedHandler);
			ArchivesDelegates->OnArchiveIndexChanged.RemoveDynamic(this, &UUI_MainScreen::OnArchiveIndexChangedHandler);

		}
	}

	{
		// 订阅者行为：注册监听事件
		USettingDelegate* SettingsDelegates = USettingDelegate::GetInstance();
		if (SettingsDelegates)
		{
			// 关键代码：订阅者将自己的处理函数绑定到代理
			SettingsDelegates->OnSettingsClosed.RemoveDynamic(this, &UUI_MainScreen::OnArchivesOrSettingsClosedHandler);
		}
	}
}



void UUI_MainScreen::OnArchivesOrSettingsClosedHandler()
{
	ShowGameMenu();
}

void UUI_MainScreen::OnArchiveIndexChangedHandler(int32 NewSlotIndex)
{	
	// 同步到GameInstance（供GameMode读取）
	if (UTD_GameInstance* GI = GetGameInstance<UTD_GameInstance>())
	{
		GI->SetCurrentSaveSlotNumber(NewSlotIndex);
	}
}