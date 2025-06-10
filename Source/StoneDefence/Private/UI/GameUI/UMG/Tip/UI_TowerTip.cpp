// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Tip/UI_TowerTip.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Data/Core/CharacterData.h"
#include "Components/MultiLineEditableTextBox.h"

void UUI_TowerTip::InitTip(const FCharacterData& InData) {
	TowerLevel->SetText(FText::AsNumber(InData.Level));
	TowerName->SetText(FText::FromName(InData.Name));
	TowerCost->SetText(FText::AsNumber(InData.Gold));
	TowerHealth->SetText(FText::AsNumber(InData.GetMaxHealth()));
	TowerPhysicalAttack->SetText(FText::AsNumber(InData.GetAttack()));
	TowerArmor->SetText(FText::AsNumber(InData.GetArmor()));
	TowerAttackSpeed->SetText(FText::AsNumber(InData.GetAttackSpeed()));
	TowerExpBar->SetPercent(InData.GetExpPercentage());
	TowerIntroduction->SetText(InData.Introduction);
}
