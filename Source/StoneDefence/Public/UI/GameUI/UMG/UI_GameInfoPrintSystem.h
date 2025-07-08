// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"
#include "UI_GameInfoPrintSystem.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_GameInfoPrintSystem : public UUI_NativeOnDrop
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TDGameLog;

public:
	virtual void NativeConstruct();

};
