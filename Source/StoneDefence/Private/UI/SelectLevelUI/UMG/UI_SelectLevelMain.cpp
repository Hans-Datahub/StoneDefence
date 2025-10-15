// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectLevelUI/UMG/UI_SelectLevelMain.h"
#include "Components/CanvasPanel.h"
#include "UI/SelectLevelUI/UMG/Button/UI_LevelButton.h"
#include "Components/Button.h"
#include "Kismet/GamePlayStatics.h"
#include "Engine/Engine.h"

void UUI_SelectLevelMain::NativeConstruct() {
	Super::NativeConstruct();

	InitSelectLevelButton();

	ReturnMenuButton->OnClicked.AddDynamic(this, &UUI_SelectLevelMain::ReturnMenu);
}

void UUI_SelectLevelMain::InitSelectLevelButton() {
	if (SelectMap) {
		TArray<UPanelSlot*>PanelSlotArray = SelectMap->GetSlots();
		for (UPanelSlot* PanelSlotEle : PanelSlotArray) {
			if (UUI_LevelButton* LevelButtonArp = Cast<UUI_LevelButton>(PanelSlotEle->Content)) {
				AllLevelButton.Add(LevelButtonArp);
			}
		}

	}
}

void UUI_SelectLevelMain::ReturnMenu() {
	UGameplayStatics::OpenLevel(GetWorld(), "SM_HallMap");
}


