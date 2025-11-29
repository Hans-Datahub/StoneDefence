// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Save/PlayerSaveData.h"
#include "Core/GameCore/TD_GameState.h"
#include "Data/Save/GameSaveData.h"


bool UPlayerSaveData::IsValid() {
	return true;
}

void UPlayerSaveData::InitSaveGame(UWorld* InWorld)
{
	//在创建新的游戏存档时，若未读取任何存档，则会调用本函数进行数据初始化

}

void UPlayerSaveData::InitSaveGameFromArchives(UWorld* InWorld)
{
	//在创建新的游戏存档时，若未读取任何存档，则会调用本函数进行数据初始化

}