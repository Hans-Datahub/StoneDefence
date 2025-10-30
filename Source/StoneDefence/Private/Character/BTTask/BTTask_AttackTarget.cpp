// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTTask/BTTask_AttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/AIController/MilitiaAIController.h"
#include "Character/AIController/MarineAIController.h"

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);


	if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner())) {//多态获取Milita和Marine的AIController
		if (UBlackboardComponent * BlackBoard = OwnerComp.GetBlackboardComponent()) {
			if (Blackboard_Actor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
				if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
					if (ARuleOfCharacter* TargetUnit = Cast<ARuleOfCharacter>(BlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName))) {
						Character->Isattack = true;

						////判断用的是Militia还是Marine的AIController
						//if(AMilitiaAIController* MilitiaAIController = Cast<AMilitiaAIController>(AIController))
						//	MilitiaAIController->AttackTarget(TargetUnit);
						//else if(AMarineAIController* MarineAIController = Cast<AMarineAIController>(AIController))
						//	MarineAIController->AttackTarget(TargetUnit);
						return EBTNodeResult::Succeeded;
					}
					else {
						Character->Isattack = false;
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;

}

void UBTTask_AttackTarget::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset()) {
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
	else {
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't Initialize task:%s"), *GetName());
	}
}