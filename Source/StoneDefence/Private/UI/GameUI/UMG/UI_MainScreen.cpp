// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "UI/GameUI/UMG/Tip/UI_TowerTip.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Components/Image.h"
#include "UI/GameUI/UMG/UI_RucksackSystem.h"
#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"


class USD_DragDropOperation;

void UUI_MainScreen::NativeConstruct() {
	Super::NativeConstruct();
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


