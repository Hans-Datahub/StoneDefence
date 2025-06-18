// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/GameUI/UMG/UI_MainScreen.h"
#include "RuleOfHUD.generated.h"

class UUI_NativeOnDrop;

UCLASS()
class STONEDEFENCE_API ARuleOfHUD : public AHUD
{
	GENERATED_BODY()
public:
	ARuleOfHUD();
	virtual void BeginPlay()override;
	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);

private:
	TSubclassOf<UUI_MainScreen> MainScreenClass;
	TSubclassOf<UUI_NativeOnDrop>NativeOnDropClass;
	UUI_MainScreen* MainScreen;
	UUI_NativeOnDrop* NativeOnDropPtr;
};
