// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTTask/BTTask_SteeringMove.h"
#include "Character/SteeringBehavior/SteeringBehaviorComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UBTTask_SteeringMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!BlackBoard) return EBTNodeResult::Failed;

    ARuleOfCharacter* TargetUnit = Cast<ARuleOfCharacter>(BlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName));
    if (!TargetUnit)return EBTNodeResult::Failed;

    USteeringBehaviorComponent* SteeringComponent = Cast<USteeringBehaviorComponent>(
        TargetUnit->GetComponentByClass(USteeringBehaviorComponent::StaticClass()));

    // 使用Steering组件计算转向力并移动角色（复刻原生MoveTo TaskNode）
    if (SteeringComponent)
    {
        FVector TargetLocation = BlackBoard->GetValueAsVector(Blackboard_TargetLocation.SelectedKeyName);
        //计算路径和转向力
        SteeringComponent->CalculatePathToLocation(TargetLocation);
        FVector SteeringForce = SteeringComponent->CalculateSteeringForce(TargetLocation);

        //应用转向力
        if (!SteeringForce.IsZero())
        {   
            TargetUnit->AddMovementInput(SteeringForce);
            //让角色自动朝向移动方向
            UCharacterMovementComponent* MoveComp = TargetUnit->GetCharacterMovement();
            MoveComp->Velocity = SteeringForce;


            MoveComp->bOrientRotationToMovement = true;//开启
            MoveComp->RotationRate = FRotator(0, 1080, 0);//设置速度
        }
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}



void UBTTask_SteeringMove::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset()) {
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
	else {
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't Initialize task:%s"), *GetName());
	}
}