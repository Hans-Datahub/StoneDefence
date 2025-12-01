// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RenderingUtils.h"
#include "ArchivesScreenshotTypes.h"  // 包含委托定义


struct FSaveSlot;
struct FSaveSlotList;

class SIMPLEARCHIVES_API ISimpleArchivesInterface
{
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	ISimpleArchivesInterface();
	virtual ~ISimpleArchivesInterface();


	virtual int32 GetSaveSlotNumber()const { return 20; }; //获取槽

	virtual	bool SaveGameData(int32 SaveNumber, FArchivesScreenshotComplete OnScreenshotDone) = 0;

	virtual	void SetSaveNumber(int32 SaveNumber) = 0;

	virtual	bool ClearGameData(int32 SaveNumber) = 0;

	virtual bool IsSlotValid(int32 SaveNumber) const = 0;

	virtual bool ReadGameData(int32 SaveNumber) = 0;

	virtual FSaveSlot *GetSaveSlot(int32 SaveNumber) = 0;

	virtual bool OpenLevel(int32 SaveNumber) = 0;

	virtual FSaveSlotList *GetSlotList() = 0;

	virtual class UWorld *GetSimpleWorld()const = 0;
};
