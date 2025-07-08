// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "GameFramework/InputSettings.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/GameUI/UMG/Skill/UI_SkillSlot.h"


void UUI_PlayerSkillSystem::NativeConstruct() {
	Super::NativeConstruct();

	//教程原写法（报错：应将ActionMappings改为GetActionMappings，改完后扔报错：.FindByPredicate”的左边必须有类/结构/联合）
/*	    auto KeyMapping = GetDefault<UInputSettings>()->GetActionMappings().FindByPredicate(
		[&](const FInputActionKeyMapping& EntryUI) {
		return (EntryUI.ActionName == "FreezeSkill");}); */ 

	LayoutPlayerSkillSlot();
}

void UUI_PlayerSkillSystem::LayoutPlayerSkillSlot() {
	if (SkillSlotClass) {
		for (const auto& Temp : GetPlayerState()->GetPlayerSkillDataID()) {
			if (UUI_SkillSlot* SkillSlot = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {				
				SkillSlot->GUID = *Temp;		
				if (UHorizontalBoxSlot* PanelSlot = SkillList->AddChildToHorizontalBox(SkillSlot)) {
					PanelSlot->SetPadding(20.f);
					PanelSlot->SetSize(ESlateSizeRule::Fill);
					PanelSlot->SetHorizontalAlignment(HAlign_Fill);
					PanelSlot->SetVerticalAlignment(VAlign_Fill);
				}				
			}
		}
	}
}
