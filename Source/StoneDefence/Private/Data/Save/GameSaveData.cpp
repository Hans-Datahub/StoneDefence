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
	CurrentConstructionTowersCD = 5.0f;
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