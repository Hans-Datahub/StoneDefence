// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_HandleLocationToMoveFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UBTService_HandleLocationToMoveFromPlayer : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerCompm, uint8* Nodememory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_LocationToMove;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_HasMoveOrder;

private:
	bool bIsLastLocationChanged = false;

	
public:
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
};
