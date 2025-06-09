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
				//������������Ŀ�겻�Ǿ�Ŀ��
				if (MonsterAIController->Target != NewTarget) {
					if (ARuleOfCharacter* MonsterSelf = Cast<ARuleOfCharacter>(MonsterAIController->GetPawn())) {
						//����Ѱ����Ŀ��ֹͣ��ǰ������������ִ���굱ǰ����ŻῪʼ�»
						MonsterSelf->GetCharacterMovement()->StopMovementImmediately();
					}
					//����Ŀ��
					MonsterAIController->Target = NewTarget;
				}

				if (MonsterAIController->Target.IsValid()) {
					if (MonsterAIController->Target->IsActive()) {
						//���������֮����루������
						FVector Direction = MonsterAIController->GetPawn()->GetActorLocation() - MonsterAIController->Target.Get()->GetActorLocation();
						Direction.Normalize();//������λ���Եõ�����
						FVector LocationWithGap = Direction * 300.f + MonsterAIController->Target.Get()->GetActorLocation();//��������մ������׷��λ��

						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, MonsterAIController->Target.Get());//Get()���ڽ���ָ����ָ��Targetת���ɶ���ʵ��
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