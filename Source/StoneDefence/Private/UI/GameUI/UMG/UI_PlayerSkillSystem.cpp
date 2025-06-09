// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_PlayerSkillSystem.h"
#include "GameFramework/InputSettings.h"


void UUI_PlayerSkillSystem::NativeConstruct() {
	Super::NativeConstruct();

	//�̳�ԭд��������Ӧ��ActionMappings��ΪGetActionMappings��������ӱ���.FindByPredicate������߱�������/�ṹ/���ϣ�
	    auto KeyMapping = GetDefault<UInputSettings>()->GetActionMappings().FindByPredicate(
		[&](const FInputActionKeyMapping& EntryUI) {
		return (EntryUI.ActionName == "FreezeSkill");});  
}