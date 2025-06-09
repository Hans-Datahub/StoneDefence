// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_GameMenuSystem.h"
#include "Components/Button.h"

void UUI_GameMenuSystem::NativeConstruct() {
	Super::NativeConstruct();

	ReturnGameButton  ->OnClicked.AddDynamic(this, &UUI_GameMenuSystem::ReturnGame);
	SaveGameButton    ->OnClicked.AddDynamic(this, &UUI_GameMenuSystem::SaveGame);
	GameSettingsButton ->OnClicked.AddDynamic(this, &UUI_GameMenuSystem::GameSettings);
	GameQuitButton    ->OnClicked.AddDynamic(this, &UUI_GameMenuSystem::GameQuit);
}

void UUI_GameMenuSystem::ReturnGame() {

}

void UUI_GameMenuSystem::SaveGame() {

}

void UUI_GameMenuSystem::GameSettings() {

}

void UUI_GameMenuSystem::GameQuit() {

}