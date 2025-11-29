// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTTask/IdleSeparation.h"
#include "Character/SteeringBehavior/SteeringBehaviorComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UIdleSeparation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!BlackBoard) return EBTNodeResult::Failed;

    ARuleOfCharacter* Owner = Cast<ARuleOfCharacter>(BlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName));
    if (!Owner)return EBTNodeResult::Failed;

    USteeringBehaviorComponent* SteeringComponent = Cast<USteeringBehaviorComponent>(
        Owner->GetComponentByClass(USteeringBehaviorComponent::StaticClass()));
    if(!SteeringComponent)return EBTNodeResult::Failed;



    FVector SeparateForce = SteeringComponent->Separation().GetSafeNormal();
    if (SeparateForce.SizeSquared() > 0.1f)
    {
        ACharacter* CharacterOwner = Cast<ACharacter>(Owner);
        if (CharacterOwner)
        {
            // 应用较小的避让力，避免过度移动
            CharacterOwner->AddMovementInput(SeparateForce);

            //让角色自动朝向移动方向
            UCharacterMovementComponent* MoveComp = CharacterOwner->GetCharacterMovement();
            MoveComp->Velocity = SeparateForce * SteeringComponent->IdleSeparationWeight * 10;
            //MoveComp->bOrientRotationToMovement = true;//开启
            //MoveComp->RotationRate = FRotator(0, 1080, 0);//设置速度
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
    