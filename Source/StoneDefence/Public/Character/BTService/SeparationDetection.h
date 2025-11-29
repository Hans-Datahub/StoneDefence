// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/SteeringBehavior/SteeringBehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SeparationDetection.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API USeparationDetection : public UBTService
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        struct FBlackboardKeySelector Blackboard_SelfActor;

    UPROPERTY(EditAnywhere)
        struct FBlackboardKeySelector Blackboard_NeedToAvoid;

    USeparationDetection()
    {
        // 设置执行间隔（毫秒）
        NodeName = TEXT("Separation Detection");
        Interval = 0.1f; // 每0.1秒执行一次
        RandomDeviation = 0.05f;
    }

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

    virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

    virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

    virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}
};
