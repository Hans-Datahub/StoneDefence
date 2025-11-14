// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterCore/Monsters.h"
#include "Militia.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AMilitia : public AMonsters
{
	GENERATED_BODY()

	virtual void BeginDestroy()override;
	void ClearAnimNotifyQueue(UAnimInstance* AnimInstance);

public:
	virtual ETeam GetTeamType() override;
	virtual void RegisterTeam() override;
	
};
