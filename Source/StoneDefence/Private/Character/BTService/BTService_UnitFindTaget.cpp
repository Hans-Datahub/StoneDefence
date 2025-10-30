// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BTService/BTService_UnitFindTaget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/AIController/MilitiaAIController.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBTService_UnitFindTaget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, Nodememory, DeltaSeconds);

	if (AMilitiaAIController* AIController = Cast<AMilitiaAIController>(OwnerComp.GetOwner())) {
		if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent()) {
			if (ARuleOfCharacter* NewTarget = Cast<ARuleOfCharacter>(AIController->FindTarget())) {
				//---若新搜索到的目标为新目标---//
				if (AIController->Target != NewTarget) {
					if (ARuleOfCharacter* UnitSelf = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
						//当搜寻到新目标停止当前活动，否则会优先执行完当前任务才会开始新活动
						UnitSelf->GetCharacterMovement()->StopMovementImmediately();
					}
					//更换目标
					AIController->Target = NewTarget;
				}

				//---若目标没变，则计算带不包括攻击范围的距离---//
				if (AIController->Target.IsValid()) {
					if (AIController->Target->IsActive()) {
						////计算怪与塔之间距离（向量）
						//FVector Direction = AIController->GetPawn()->GetActorLocation() - AIController->Target.Get()->GetActorLocation();

						//FVector NormalizedDirection = Direction.GetSafeNormal();
						//FVector NormalizedDirection2 = NormalizedDirection * -300.f;
						//
						////FVector LocationWithGap = Direction * -300.f + AIController->Target.Get()->GetActorLocation();//计算出最终带间隔的追踪位置
						//FVector LocationWithGap = Direction  + NormalizedDirection2;//计算出最终带间隔的追踪位置

						FVector TargetLocation = AIController->Target.Get()->GetActorLocation();
						FVector SelfLocation = AIController->GetPawn()->GetActorLocation();

						// 计算从目标指向自身的单位方向
						FVector DirectionToSelf = (SelfLocation - TargetLocation).GetSafeNormal();

						// 从目标位置反向偏移300单位
						FVector LocationWithGap = TargetLocation + (DirectionToSelf * 2000.0f);




						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, AIController->Target.Get());//Get()用于将弱指针所指的Target转化成对象实例
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


			if (AIController->Target.IsValid()) {
				
				FVector Mylocation = AIController -> GetPawn()->GetActorLocation();
				FVector TMDistance = Mylocation - AIController->Target->GetActorLocation();

				if (TMDistance.Size() > 2200) {
					if (ARuleOfCharacter* UnitAI = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
						UnitAI->Isattack = false;
					}
				}
				else {
					if (ARuleOfCharacter* UnitAI = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
						UnitAI->Isattack = true;
					}
				}
				MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, TMDistance.Size());
			}
			else {
				MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, 0.0f);

			}

			//临时设置最终目标位置  测试
			MyBlackBoard->SetValueAsVector(BlackBoardKey_FinalTargetLocation.SelectedKeyName, FVector(-7000,-7620,0));
		}
	}
}