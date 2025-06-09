// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/Core/RuleOfAIController.h"
#include "Character/Core/RuleOfCHaracter.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AMonsterAIController : public ARuleOfAIController
{
	GENERATED_BODY()
	
public:
	virtual AActor* FindTarget();
	virtual void AttackTarget(class ARuleOfCharacter* AttackTarget);


};
