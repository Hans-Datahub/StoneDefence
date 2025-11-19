// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CalculateDegreeToFaceTarget.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UCalculateDegreeToFaceTarget : public UBTService
{
	GENERATED_BODY()
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) override;

public:

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_DegreeToFaceTarget;

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
};
