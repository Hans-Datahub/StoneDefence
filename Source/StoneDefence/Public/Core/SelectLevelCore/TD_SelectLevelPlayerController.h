// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TD_SelectLevelPlayerController.generated.h"

class ATD_GameCamera;

UCLASS()
class STONEDEFENCE_API ATD_SelectLevelPlayerController : public APlayerController
{
	GENERATED_BODY()

	void SetupInputComponent();
	void BeginPlay();

	ATD_GameCamera* GetTargetCameraByIndex();
	void CameraMove();
	void NavigateLeft();
	void NavigateRight();
	void ConfirmSelection();

	TArray<AActor*> FoundCameras;
	int32 CurrentSelectedLevelIndex = 1;
	bool isCameraMoving = false;

	FTimerHandle TimerHandle;
};
