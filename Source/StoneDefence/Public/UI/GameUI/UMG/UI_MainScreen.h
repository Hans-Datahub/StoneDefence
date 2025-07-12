// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"
#include "UI_MainScreen.generated.h"


UCLASS()
class STONEDEFENCE_API UUI_MainScreen : public UUI_NativeOnDrop
{
	GENERATED_BODY()

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

public:
	virtual void NativeConstruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);
	void UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD);

};
