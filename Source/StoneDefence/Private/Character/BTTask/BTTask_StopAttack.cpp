// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTTask/BTTask_StopAttack.h"
#include "Character/Core/RuleOfCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_StopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	ARuleOfCharacter* TargetUnit = Cast<ARuleOfCharacter>(BlackBoard->GetValueAsObject(Blackboard_Actor.SelectedKeyName));
	if (!TargetUnit)return EBTNodeResult::Failed;

	TargetUnit->Isattack = false;
	return EBTNodeResult::Succeeded;
}

void UBTTask_StopAttack::InitializeFromAsset(UBehaviorTree& Asset) {
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset()) {
		Blackboard_Actor.ResolveSelectedKey(*BBAsset);
	}
	else {
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't Initialize task:%s"), *GetName());
	}
}