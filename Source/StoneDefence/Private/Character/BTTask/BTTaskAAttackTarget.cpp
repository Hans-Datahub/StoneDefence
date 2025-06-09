// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTTask/BTTaskAAttackTarget.h"
#include "Character/AIController/MonsterAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Core/RuleOfCharacter.h"

EBTNodeResult::Type UBTTaskAAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AMonsterAIController* MyAIController = Cast<AMonsterAIController>(OwnerComp.GetOwner())) {
		if (UBlackboardComponent * MyBlackBoard = OwnerComp.GetBlackboardComponent()) {
			if (Blackboard_Actor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
				if (ARuleOfCharacter* MyPawn = Cast<ARuleOfCharacter>(MyAIController->GetPawn())) {
					if (ARuleOfCharacter* TargetTower = Cast<ARuleOfCharacter>(MyBlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName))) {
						MyPawn->Isattack = true;
						MyAIController->AttackTarget(TargetTower);
						return EBTNodeResult::Succeeded;
					}
					else {
						MyPawn->Isattack = false;
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;

}

void UBTTaskAAttackTarget::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset()) {
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
	else {
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't Initialize task:%s"), *GetName());
	}
}