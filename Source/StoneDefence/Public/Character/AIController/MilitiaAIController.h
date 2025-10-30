// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AIController/MonsterAIController.h"
#include "MilitiaAIController.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AMilitiaAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	virtual AActor* FindTarget();
};
