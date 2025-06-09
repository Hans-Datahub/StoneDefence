// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveSlotList.generated.h"

USTRUCT()
struct FSaveSlot
{
	GENERATED_BODY()

	FSaveSlot();

	UPROPERTY(SaveGame)
	FText DataText;

	UPROPERTY(SaveGame)
	bool isSaveSlotEmpty;//bSave->isSaveSlotEmpty
};

USTRUCT()
struct FSaveSlotList
{

	GENERATED_BODY()

	FSaveSlotList();

	UPROPERTY(SaveGame)
	TMap<int32, FSaveSlot> Slots;

	TMap<int32, float> CompletionStatus;//DegreeOfCompletion->CompletionStatus

	int32 GetMinialIndexOfUnsavedSlot();

	bool AddGameData(int32 SlotIndex);//AddGameDataByNumber->AddGameData   SlotNumber->SlotIndex
	bool RemoveGameData(int32 SlotIndex);//RemoveAtGameData->RemoveGameData
};

UCLASS()
class STONEDEFENCE_API UGameSaveSlotList :public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FSaveSlotList SlotList;
};