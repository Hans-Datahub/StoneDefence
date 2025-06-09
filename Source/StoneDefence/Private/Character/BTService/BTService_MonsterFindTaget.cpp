// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BTService/BTService_MonsterFindTaget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/AIController/MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBTService_MonsterFindTaget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, Nodememory, DeltaSeconds);

	if (ARuleOfAIController* MonsterAIController = Cast<ARuleOfAIController>(OwnerComp.GetOwner())) {
		if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent()) {
			if (ARuleOfCharacter* NewTarget = Cast<ARuleOfCharacter>(MonsterAIController->FindTarget())) {
				//若新搜索到的目标不是旧目标
				if (MonsterAIController->Target != NewTarget) {
					if (ARuleOfCharacter* MonsterSelf = Cast<ARuleOfCharacter>(MonsterAIController->GetPawn())) {
						//当搜寻到新目标停止当前活动，否则会优先执行完当前任务才会开始新活动
						MonsterSelf->GetCharacterMovement()->StopMovementImmediately();
					}
					//更换目标
					MonsterAIController->Target = NewTarget;
				}

				if (MonsterAIController->Target.IsValid()) {
					if (MonsterAIController->Target->IsActive()) {
						//计算怪与塔之间距离（向量）
						FVector Direction = MonsterAIController->GetPawn()->GetActorLocation() - MonsterAIController->Target.Get()->GetActorLocation();
						Direction.Normalize();//向量单位化以得到方向
						FVector LocationWithGap = Direction * 300.f + MonsterAIController->Target.Get()->GetActorLocation();//计算出最终带间隔的追踪位置

						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, MonsterAIController->Target.Get());//Get()用于将弱指针所指的Target转化成对象实例
						MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, LocationWithGap);
					}	 
					else { MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
						   MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
					}
				}
				else { MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
					   MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
				}
			}
			else {
				MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
				MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
			}

			if (MonsterAIController->Target.IsValid()) {
				
				FVector Mylocation = MonsterAIController -> GetPawn()->GetActorLocation();
				FVector TMDistance = Mylocation - MonsterAIController->Target->GetActorLocation();

				if (TMDistance.Size() > 2200) {
					if (ARuleOfCharacter* MonsterAI = Cast<ARuleOfCharacter>(MonsterAIController->GetPawn())) {
						MonsterAI->Isattack = false;
					}
				}
				else {
					if (ARuleOfCharacter* MonsterAI = Cast<ARuleOfCharacter>(MonsterAIController->GetPawn())) {
						MonsterAI->Isattack = true;
					}
				}
				MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, TMDistance.Size());
			}
			else {
				MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, 0.0f);

			}
			
		}
	}
}