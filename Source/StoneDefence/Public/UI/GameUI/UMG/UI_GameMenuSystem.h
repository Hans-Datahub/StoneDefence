// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_GameMenuSystem.generated.h"

class UButton;

UCLASS()
class STONEDEFENCE_API UUI_GameMenuSystem : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* ReturnGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GameSettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GameQuitButton;

public:
	virtual void NativeConstruct();

private:
	UFUNCTION()
	void ReturnGame();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void GameSettings();

	UFUNCTION()
	void GameQuit();

};
