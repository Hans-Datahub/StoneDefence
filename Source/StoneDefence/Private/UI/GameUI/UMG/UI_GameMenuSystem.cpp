// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_GameMenuSystem.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "Components/Button.h"

void UUI_GameMenuSystem::NativeConstruct() {
	Super::NativeConstruct();

	GameQuitButton->OnClicked.AddDynamic(this, &UUI_GameMenuSystem::GameQuit);
}



void UUI_GameMenuSystem::GameQuit() {
	//关闭控件
	if (this) {
		this->RemoveFromParent();
	}

	//开启弹窗，是否确认
	//
	//

	//切换关卡
	UGameplayStatics::OpenLevel(GetWorld(), "SM_HallMap");
}

void UUI_GameMenuSystem::BindSaveGame(FOnButtonClickedEvent ClickedEvent)
{
	SaveGameButton->OnClicked = ClickedEvent;
}

void UUI_GameMenuSystem::BindSaveSettings(FOnButtonClickedEvent ClickedEvent)
{
	GameSettingsButton->OnClicked = ClickedEvent;
}

void UUI_GameMenuSystem::BindReturnGame(FOnButtonClickedEvent ClickedEvent)
{
	ReturnGameButton->OnClicked = ClickedEvent;
}