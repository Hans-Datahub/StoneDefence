// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "TD_SelectLevelHUD.generated.h"


class UUI_SelectLevelMain;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATD_SelectLevelHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATD_SelectLevelHUD();
	virtual void BeginPlay()override;

private:
	TSubclassOf<UUI_SelectLevelMain> SelectLevelMainClass;
	UUI_SelectLevelMain* SelectLevelMain;
	
};
