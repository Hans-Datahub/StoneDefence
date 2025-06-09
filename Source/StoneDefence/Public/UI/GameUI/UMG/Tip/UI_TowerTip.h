// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_TowerTip.generated.h"

class UTextBlock;
class UProgressBar;
class UMultiLineEditableTextBox;
struct FCharacterData;

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_TowerTip : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerLevel;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerCost;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerHealth;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerPhysicalAttack;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerArmor;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerAttackSpeed;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* TowerExpBar;
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TowerIntroduction;

public:
	void InitTip(const FCharacterData& InData);
};
