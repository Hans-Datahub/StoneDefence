// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/Core/RuleOfHUD.h"
#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"

ARuleOfHUD::ARuleOfHUD() {
	
	static ConstructorHelpers::FClassFinder<UUI_MainScreen>GameMain_BPClass(TEXT("/Game/UI/Game/MainScreen_BP"));
	
	MainScreenClass = GameMain_BPClass.Class;

	static ConstructorHelpers::FClassFinder<UUI_NativeOnDrop>NativeOnDrop_BPClass(TEXT("/Game/UI/Game/Drop/Drop_BP"));

	NativeOnDropClass = GameMain_BPClass.Class;

}

void ARuleOfHUD::BeginPlay() {
	Super::BeginPlay();

	MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
	MainScreen->AddToViewport(2);

	NativeOnDropPtr = CreateWidget<UUI_NativeOnDrop>(GetWorld(), NativeOnDropClass);
	NativeOnDropPtr->AddToViewport(0);
}

void ARuleOfHUD::UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD) {
	if (MainScreen) {
		MainScreen->UpdateInventorySlot(InventorySlotGUID, bInCD);
	}
}

void ARuleOfHUD::UpdatePlayerSkillSlot(const FGuid& PlayerSkillSlotGUID, bool bInCD) {
	if (MainScreen) {
		MainScreen->UpdatePlayerSkillSlot(PlayerSkillSlotGUID, bInCD);
	}
}
