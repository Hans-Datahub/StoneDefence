// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SelectLevelCore/TD_SelectLevelGameMode.h"
#include "UI/SelectLevelUI/Core/TD_SelectLevelHUD.h"

ATD_SelectLevelGameMode::ATD_SelectLevelGameMode() {
	HUDClass = ATD_SelectLevelHUD::StaticClass();
}

void ATD_SelectLevelGameMode::BeginPlay() {

}
