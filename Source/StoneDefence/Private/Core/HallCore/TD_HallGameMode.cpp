// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HallCore/TD_HallGameMode.h"
#include "UI/HallUI/Core/TD_HallHUD.h"
#include "Core/HallCore/TD_HallPawn.h"
#include "Core/HallCore/TD_HallPlayerController.h"
#include "Core/HallCore/TD_HallGameState.h"
#include "Core/RuleOfThePlayerState.h"

#include "Core/GameCore/TD_GameCamera.h"
#include "Kismet/GameplayStatics.h"


ATD_HallGameMode::ATD_HallGameMode() {
	HUDClass = ATD_HallHUD::StaticClass();
	DefaultPawnClass = ATD_HallPawn::StaticClass();
	PlayerControllerClass = ATD_HallPlayerController::StaticClass();
	GameStateClass = ATD_HallGameState::StaticClass();
	PlayerStateClass = ARuleOfThePlayerState::StaticClass();
}


void ATD_HallGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 查找场景中的自定义摄像机
    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATD_GameCamera::StaticClass(), FoundCameras);

    if (FoundCameras.Num() > 0)
    {
        ATD_GameCamera* Camera = Cast<ATD_GameCamera>(FoundCameras[0]);
        if (Camera && GetWorld()->GetFirstPlayerController())
        {
            // 设置这个摄像机为视图目标
            GetWorld()->GetFirstPlayerController()->SetViewTarget(Camera);
        }
    }
}