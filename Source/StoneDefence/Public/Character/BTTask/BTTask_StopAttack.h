// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopAttack.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UBTTask_StopAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset)override;

	UPROPERTY()
		struct FBlackboardKeySelector Blackboard_Actor;

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
	
};
