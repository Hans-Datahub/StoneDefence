// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_LevelButton.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_LevelButton : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UBorder* LevelSelectBorder;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LevelProBar;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextLevelButton;

public:
	virtual void NativeConstruct();

	UFUNCTION()
	void SelectLevel();
};
