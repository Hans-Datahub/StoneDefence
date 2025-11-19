// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTService/CalculateDegreeToFaceTarget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AIController/MonsterAIController.h"
#include "Character/Core/RuleOfCharacter.h"

void UCalculateDegreeToFaceTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, Nodememory, DeltaSeconds);

	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (!BlackBoard) return;
	AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetOwner());
	if (!AIController) return;
	ARuleOfCharacter* Target = Cast<ARuleOfCharacter>(AIController->FindTarget());
	if (!Target) return; 
	if (Target->IsDeath()) return; 
	ARuleOfCharacter* UnitSelf = Cast<ARuleOfCharacter>(AIController->GetPawn());
	if (!UnitSelf) return;

	FVector SelfForward = UnitSelf->GetActorForwardVector(); // 当前前向向量
	SelfForward.Z = 0.0f;

	FVector TargetLocation = Target->GetActorLocation();
	FVector SelfLocation = UnitSelf->GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - SelfLocation).GetSafeNormal(); // 从AI到目标的方向向量

	// 计算两个向量的夹角（弧度转角度）
	// 点积公式：cosθ = (A·B) / (|A|·|B|)，因向量已归一化，分母为1
	float DotProduct = FVector::DotProduct(SelfForward, DirectionToTarget);
	// 防止精度问题导致DotProduct超出[-1,1]范围（可能引发acos错误）
	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);

	float RadianError = FMath::Acos(DotProduct); // 弧度误差
	float DegreeError = FMath::RadiansToDegrees(RadianError); // 转换为角度

	// 将角度误差存入黑板
	BlackBoard->SetValueAsFloat(BlackBoardKey_DegreeToFaceTarget.SelectedKeyName, DegreeError);

	//Degbug
	UE_LOG(LogTemp, Log, TEXT("CalculateDegreeFacingTarget: %s is %f Degree off To Target"), *UnitSelf->GetName(), DegreeError);


}