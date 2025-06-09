// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/UMG/UI_HallMenuSystem.h"
#include "Kismet/GameplayStatics.h"

void UUI_HallMenuSystem::NativeConstruct() {
	Super::NativeConstruct();
}

void UUI_HallMenuSystem::BindGameStart(FOnButtonClickedEvent ClickedEvent) {
	GameStartButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindHistory(FOnButtonClickedEvent ClickedEvent) {
	HistoryButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindGameSettings(FOnButtonClickedEvent ClickedEvent) {
	GameSettingsButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindTutorialWebsite(FOnButtonClickedEvent ClickedEvent) {
	TutorialWebsiteButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindBrowser(FOnButtonClickedEvent ClickedEvent) {
	BrowserButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindQuitGame(FOnButtonClickedEvent ClickedEvent) {
	QuitGameButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindSecretTerritory(FOnButtonClickedEvent ClickedEvent) {
	SecretTerritoryButton->OnClicked = ClickedEvent;
}

void UUI_HallMenuSystem::BindSpecialContent(FOnButtonClickedEvent ClickedEvent) {
	SpecialContentButton->OnClicked = ClickedEvent;
}
