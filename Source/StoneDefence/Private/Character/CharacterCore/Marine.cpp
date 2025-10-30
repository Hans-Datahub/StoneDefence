// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Marine.h"

ETeam AMarine::GetTeamType() {
	return ETeam::BLUE;
}                              

void AMarine::RegisterTeam() {
	GetCharacterData().Team == ETeam::BLUE;
}