// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterCore/Monsters.h"
#include "Marine.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AMarine : public AMonsters
{
	GENERATED_BODY()
	
	virtual ETeam GetTeamType() override;
	virtual void RegisterTeam() override;
};
