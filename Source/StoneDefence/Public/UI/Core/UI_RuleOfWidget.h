// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/GameCore/TD_GameState.h"
#include "Core/GameCore/TD_PlayerState.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "UI_RuleOfWidget.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_RuleOfWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UUI_RuleOfWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	FGuid GUID;

	class ATD_GameState* GetGameState();
	class ATD_PlayerState* GetPlayerState();

	class ATD_PlayerController* GetPlayerController();
};
