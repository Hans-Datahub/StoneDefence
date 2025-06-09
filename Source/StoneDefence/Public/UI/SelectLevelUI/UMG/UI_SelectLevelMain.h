// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SelectLevelMain.generated.h"

class UButton;
class UUI_LevelButton;
class UCanvasPanel;

UCLASS()
class STONEDEFENCE_API UUI_SelectLevelMain : public UUI_RuleOfWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnMenuButton;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SelectMap;

public:
	virtual void NativeConstruct();

protected:
	void InitSelectLevelButton();

	UFUNCTION()
	void ReturnMenu();

private:
	TArray<UUI_LevelButton*>AllLevelButton;

	
};
