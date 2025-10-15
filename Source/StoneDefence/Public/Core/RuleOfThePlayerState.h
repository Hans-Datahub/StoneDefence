// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RuleOfThePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ARuleOfThePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	bool ClearPlayerData(int32 SaveNumber);
	
	virtual bool ReadGameData(int32 SaveNumber) { return false; };
	virtual bool SaveGameData(int32 SaveNumber) { return false; };
};
