// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "GameFramework/InputSettings.h"


void UUI_PlayerSkillSystem::NativeConstruct() {
	Super::NativeConstruct();

	//教程原写法（报错：应将ActionMappings改为GetActionMappings，改完后扔报错：.FindByPredicate”的左边必须有类/结构/联合）
	    auto KeyMapping = GetDefault<UInputSettings>()->GetActionMappings().FindByPredicate(
		[&](const FInputActionKeyMapping& EntryUI) {
		return (EntryUI.ActionName == "FreezeSkill");});  
}