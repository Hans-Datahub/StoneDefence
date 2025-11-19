// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Save/GameSaveData.h"

FBuildingTowers::FBuildingTowers() {
	Init();
}


void FBuildingTowers::Init() {
	TowerID = INDEX_NONE;
	BuildingCost = INDEX_NONE;
	TowersPrepareBuildingNumber = 0.0f;
	TowersConstructionNumber = 0.0f;
	MaxConstructionTowersCD = 0.0f;
	CurrentConstructionTowersCD = 0.0f;
	Icon = NULL;
	isIconDragged = false;
	isCDFreezed = false;
	CallUpdateTowrsInfoOrNot = false;
}

float FBuildingTowers::GetTowerConstructionCDPercentage() {
	return MaxConstructionTowersCD > 0 ? CurrentConstructionTowersCD / MaxConstructionTowersCD : 0.f;
}

bool FBuildingTowers::IsValid() {
	return TowerID != INDEX_NONE;
}

void FBuildingTowers::ResetCD() {
	CurrentConstructionTowersCD = MaxConstructionTowersCD;
}


void UGameSaveData::InitSaveGame(UWorld* InWorld)
{
	//在创建新的游戏存档时，若未读取任何存档，则会调用本函数进行数据初始化

}

