// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameCore.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API USaveGameCore : public USaveGame
{
	GENERATED_BODY()
	
public:

	//创建的时候才调用，如果是读取就不会调用
	virtual void InitSaveGame(UWorld *InWorld);

	//读取存档也会调用该函数
	virtual void InitSaveGameFromArchives(UWorld *InWorld){}
};
