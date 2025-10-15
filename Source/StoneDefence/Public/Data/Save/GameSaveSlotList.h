// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/SimpleArchivesList.h"
#include "GameSaveSlotList.generated.h"


UCLASS()
class STONEDEFENCE_API UGameSaveSlotList :public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FSaveSlotList SlotList;

	virtual void InitSaveGame(UWorld* InWorld);

	virtual void InitSaveGameFromArchives(UWorld* InWorld);
};