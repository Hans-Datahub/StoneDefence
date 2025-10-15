// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TD_SelectLevelGameMode.generated.h"

class ATD_GameCamera;

UCLASS()
class STONEDEFENCE_API ATD_SelectLevelGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ATD_SelectLevelGameMode();

	virtual void BeginPlay();
	ATD_GameCamera* GetCamera(TArray<AActor*> FoundCameras, FString CameraName);
};
