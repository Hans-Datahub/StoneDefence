// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SimpleArchivesInterface.h"
#include "../StoneDefenceType.h"
#include "TD_GameInstance.generated.h"

class ARuleOfTheGameState;
/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UTD_GameInstance : public UGameInstance, public ISimpleArchivesInterface
{
	GENERATED_BODY()
	
public:
	UTD_GameInstance();

	virtual void Init() override;

	virtual int32 GetSaveSlotNumber() const;

	virtual FSaveSlotList* GetSlotList();

	virtual bool OpenLevel(int32 SaveNumber);

	virtual bool IsSlotValid(int32 SaveNumber) const;

	virtual	bool SaveGameData(int32 SaveNumber);

	virtual	bool ClearGameData(int32 SaveNumber);

	virtual bool ReadGameData(int32 SaveNumber);

	virtual	void SetSaveNumber(int32 SaveNumber);

	virtual FSaveSlot* GetSaveSlot(int32 SaveNumber);

	virtual class UWorld* GetSimpleWorld() const;

	ARuleOfTheGameState* GetGameState() const;

	class UWorld* GetSafeWorld() const;

	int32 GetCurrentSaveSlotNumber() const;

	void ClearSaveMark();

	EGameSaveType GetGameType() { return GameSaveType; };

	UFUNCTION(/*Server*/)
		void SetCurrentSaveSlotNumber(int32 InSaveSlotNumber);
protected:
	UFUNCTION(/*Server*/)
		void OpenLevelOnServer(const FText& MapName);

protected:
	UPROPERTY()
		int32 SaveSlotNumber;

	//游戏存储状态flag
	EGameSaveType GameSaveType;
};
