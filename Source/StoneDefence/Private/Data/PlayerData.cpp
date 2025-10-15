// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PlayerData.h"

FPlayerData::FPlayerData() {
	Init();
}

void FPlayerData::Init() {
	PlayerID = INDEX_NONE;
	PlayerName = NAME_None;
	GameGold = 0;
	Diamonds = 0;
	Coppers = 0;
	Team = ETeam::RED;
	GameGoldTime = 0.f;
	MaxGameGoldTime = 1.25;
	SkillID.Add(0);
}

bool FPlayerData::IsValid() {
	return PlayerID != INDEX_NONE;
}

