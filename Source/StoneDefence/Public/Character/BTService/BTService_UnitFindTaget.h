// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Character/Core/RuleOfAIController.h"
#include "BTService_UnitFindTaget.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UBTService_UnitFindTaget : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_Target;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_Distance;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_TargetLocation;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_AttackLocationForTarget;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector BlackBoardKey_FinalTargetLocation;

private:
	bool bOldIsattack = false;

public:
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task){}

	virtual void OnGameplayTaskActivated(UGameplayTask& Task){}

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}

};
