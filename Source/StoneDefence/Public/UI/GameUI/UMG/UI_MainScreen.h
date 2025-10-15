// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"
#include "ArchiveDelegate.h"
#include "UI_MainScreen.generated.h"


UCLASS()
class STONEDEFENCE_API UUI_MainScreen : public UUI_NativeOnDrop
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* NewWindows;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* BoxList;

	UPROPERTY(meta = (BindWidget))
	class UUI_GameMenuSystem* GameMenuSystem;
	
	UPROPERTY(meta = (BindWidget))
	class UUI_GameInfoPrintSystem* GameInfoPrintSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_MiniMapSystem* GameMiniMapSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_MissionSystem* MissionSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_PlayerSkillSystem* PlayerSkillSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_RucksackSystem* RucksackSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_ToolBarSystem* ToolBarSystem;

	UPROPERTY(meta = (BindWidget))
	class UUI_TowerTip* CharacterTip;

	UPROPERTY(meta = (BindWidget))
	class UImage* FireConcentrationPoint;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_ArchivesSystem> ArchivesSystemClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_GameSettingsSystem> GameSettingsClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_SimplePopup> PopupClass;

public:
	virtual void NativeConstruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);
	void UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD);


	UFUNCTION()
	void Settings();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void SaveSettings();

	UFUNCTION()
	void ReturnGame();

	UFUNCTION()
	void FreezeTimeAndCD();

	UFUNCTION()
	void UnFreezeTimeAndCD();

	//void ToggleGameMenuDisplay();
	
	void ShowGameMenu();

	void HideGameMenu();


						//--------------------------------代理部分--------------------------------//
private:
	// 订阅者核心方法：响应代理事件
	UFUNCTION()
		void OnArchivesOrSettingsClosedHandler();  // 事件处理函数

		// 订阅者管理方法
	void SubscribeToCloseEvent();    // 订阅事件
	void UnsubscribeFromCloseEvent(); // 取消订阅

};
