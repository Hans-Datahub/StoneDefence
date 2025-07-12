// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "GameFramework/InputSettings.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/GameUI/UMG/Skill/UI_SkillSlot.h"


void UUI_PlayerSkillSystem::NativeConstruct() {
	Super::NativeConstruct();

	LayoutPlayerSkillSlot();
}

void UUI_PlayerSkillSystem::LayoutPlayerSkillSlot() {
	if (SkillSlotClass) {
		const TArray<const FGuid*> PlayerSkillGUIDs = GetPlayerState()->GetPlayerSkillDataID();
		for (int32 i = 0; i < PlayerSkillGUIDs.Num(); i++) {
			if (UUI_SkillSlot* SkillSlot = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {				
				SkillSlot->GUID = *PlayerSkillGUIDs[i];
				if (UHorizontalBoxSlot* PanelSlot = SkillList->AddChildToHorizontalBox(SkillSlot)) {
					PanelSlot->SetPadding(20.f);
					PanelSlot->SetSize(ESlateSizeRule::Fill);
					PanelSlot->SetHorizontalAlignment(HAlign_Fill);
					PanelSlot->SetVerticalAlignment(VAlign_Fill);
				}	

				//技能赋值
				if (GetPlayerState()->GetPlayerData().SkillID.IsValidIndex(i)) {
					int32 CurrentID = GetPlayerState()->GetPlayerData().SkillID[i];
					GetPlayerState()->AddPlayerSkill(PlayerSkillGUIDs[i], CurrentID);
				}

				SkillSlot->UpdateUI();			
			}

			
		}
	}
}

//主要由服务端Tick调用该函数
void UUI_PlayerSkillSystem::UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD) {
	for (UPanelSlot* PanelSlot : SkillList->GetSlots()) {
		if (UUI_SkillSlot * SkillSlot = Cast<UUI_SkillSlot>(PanelSlot->Content)) {
			if (SkillSlot->GUID == PlayerSkillSlotGUID) {
				if (bInCD) {
					SkillSlot->DrawSlotCD(SkillSlot->GetPlayerSkillData()->GetSkillCDPercentage());
				}
				else {
					SkillSlot->DrawSlotCD(0.f);
				}
				SkillSlot->UpdateSlotInfo(SkillSlot->GetPlayerSkillData()->SkillNumber, SkillSlot->GetPlayerSkillData()->SkillCD);
				break;
			}
		}
	}
}