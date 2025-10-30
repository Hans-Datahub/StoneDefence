// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Militia.h"


ETeam AMilitia::GetTeamType() {
	return ETeam::RED;
}

void AMilitia::RegisterTeam() {
	GetCharacterData().Team == ETeam::RED;
}