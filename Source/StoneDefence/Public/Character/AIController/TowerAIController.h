// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/Core/RuleOfAIController.h"
#include "TowerAIController.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowerAIController : public ARuleOfAIController
{
	GENERATED_BODY()

public:
	ATowerAIController();

	virtual void Tick(float DeltaTime)override;

	virtual AActor* FindTarget();

protected:
	void BTService_FindTarget();

	virtual void AttackTarget(ARuleOfCharacter* AttackTarget);

	UPROPERTY()
	TArray<class ARuleOfCharacter*> TArrayMonsters;

	float HeartbeatDiagnosis;
};
