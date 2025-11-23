// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SteeringMove.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UBTTask_SteeringMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		struct FBlackboardKeySelector Blackboard_Actor;

	UPROPERTY(EditAnywhere)
		struct FBlackboardKeySelector Blackboard_TargetLocation;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset)override;

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
	
};
