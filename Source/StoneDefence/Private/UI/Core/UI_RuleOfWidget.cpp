// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/UI_RuleOfWidget.h"




UUI_RuleOfWidget::UUI_RuleOfWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GUID = FGuid::NewGuid();
}

ATD_GameState* UUI_RuleOfWidget::GetGameState() {
	if (GetWorld())
		return GetWorld()->GetGameState<ATD_GameState>();
	return nullptr;
}

ATD_PlayerState* UUI_RuleOfWidget::GetPlayerState() {
	if (GetPlayerController()){
		return GetPlayerController()->GetPlayerState<ATD_PlayerState>();
	}
	return nullptr;
}

ATD_PlayerController* UUI_RuleOfWidget::GetPlayerController() {
	if (GetWorld())
		return GetWorld()->GetFirstPlayerController<ATD_PlayerController>();
	return nullptr;
}
