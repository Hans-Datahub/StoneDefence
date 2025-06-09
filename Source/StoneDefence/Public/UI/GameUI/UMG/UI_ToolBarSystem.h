// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_ToolBarSystem.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class STONEDEFENCE_API UUI_ToolBarSystem : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Coins;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowerDeathNumber;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameTimeCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KilledSoldier;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SurplusQuantity;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* SurplusQuantityProgressBar;

public:
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

private:
	FString GetCurrentTimeCount(float NewTimeCount);
};
