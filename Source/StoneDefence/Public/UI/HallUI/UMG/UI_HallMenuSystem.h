// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/Core/UI_RuleOfWidget.h"
#include "Components/Button.h"
#include "UI_HallMenuSystem.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_HallMenuSystem : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* GameStartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SecretTerritoryButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HistoryButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GameSettingsButtonTD;

	UPROPERTY(meta = (BindWidget))
	UButton* TutorialWebsiteButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BrowserButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SpecialContentButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

public:
	virtual void NativeConstruct();

	void BindGameStart(FOnButtonClickedEvent ClickedEvent);
	void BindSecretTerritory(FOnButtonClickedEvent ClickedEvent);
	void BindHistory(FOnButtonClickedEvent ClickedEvent);
	void BindGameSettings(FOnButtonClickedEvent ClickedEvent);
	void BindTutorialWebsite(FOnButtonClickedEvent ClickedEvent);
	void BindBrowser(FOnButtonClickedEvent ClickedEvent);
	void BindSpecialContent(FOnButtonClickedEvent ClickedEvent);
	void BindQuitGame(FOnButtonClickedEvent ClickedEvent);
};
