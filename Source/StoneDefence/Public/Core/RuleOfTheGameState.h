// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Data/Save/GameSaveSlotList.h"
#include "RuleOfTheGameState.generated.h"

class UGameSaveSlotList;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ARuleOfTheGameState : public AGameState
{
	GENERATED_BODY()

public:
	ARuleOfTheGameState();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = SaveData)
	virtual	bool SaveGameData(int32 SaveNumber) { return false; }

	UFUNCTION(BlueprintCallable, Category = SaveData)
	virtual	bool ReadGameData(int32 SaveNumber) { return false; }

	FSaveSlotList &GetSaveSlotList();

	virtual	bool ClearGameData(int32 SaveNumber);

	virtual FSaveSlot *GetSaveSlot(int32 SaveNumber);
protected:
	UGameSaveSlotList *GetGameSaveSlotList();

protected:
	UPROPERTY()
	UGameSaveSlotList *SlotList;
};
