// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HallCore/TD_HallGameMode.h"
#include "UI/HallUI/Core/TD_HallHUD.h"
#include "Core/HallCore/TD_HallPawn.h"
#include "Core/HallCore/TD_HallPlayerController.h"

ATD_HallGameMode::ATD_HallGameMode() {
	HUDClass = ATD_HallHUD::StaticClass();
	DefaultPawnClass = ATD_HallPawn::StaticClass();
	PlayerControllerClass = ATD_HallPlayerController::StaticClass();

}