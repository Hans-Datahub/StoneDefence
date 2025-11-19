// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTService/BTService_HandleLocationToMoveFromPlayer.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/AIController/MarineAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/GameCore/LowPolyGameState.h"
#include "Character/Anim/MilitiaAnimInstance.h"


void UBTService_HandleLocationToMoveFromPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) {
	if (UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent()) {
		//获取ServiceOwner
		ARuleOfCharacter* ServiceOwner = nullptr;
		AMarineAIController* AIController = Cast<AMarineAIController>(OwnerComp.GetOwner());
		if(AIController)
			ServiceOwner = Cast<ARuleOfCharacter>(AIController->GetPawn());
		if (!ServiceOwner) return;
		if (!ServiceOwner->GUID.IsValid()) return;

		//从CharacterData获取LocationToMove
		if (GetWorld()) {
			if (ALowPolyGameState* TempGameState = GetWorld()->GetGameState<ALowPolyGameState>()) {
				UGameSaveData* TempSaveData = TempGameState->GetSaveData();
				
				if (TempSaveData->CharacterDatas.Contains(ServiceOwner->GUID)) {			
					FCharacterData& CharacterData = TempGameState->GetCharacterData(ServiceOwner->GUID);
					FVector TargetLocationToMove = CharacterData.LocationToMove;
					//传值
					BlackBoard->SetValueAsVector(BlackBoardKey_LocationToMove.SelectedKeyName, TargetLocationToMove);
					
					/*--------------*/
					//更新HasMoveOrder
					/*--------------*/
					//若位置改变，判断位 置为true
					if (LastLocationToMove != TargetLocationToMove) {
						LastLocationToMove = TargetLocationToMove;
						bIsLastLocationChanged = true;
					}
					if (bIsLastLocationChanged) {
						BlackBoard->SetValueAsBool(BlackBoardKey_HasMoveOrder.SelectedKeyName, true);
						UE_LOG(LogTemp, Log, TEXT("Task:黑板值已设置true"));
						bIsLastLocationChanged = false;//数据更新后，判断位 置false
						//动画蓝图状态更新
						if (UMilitiaAnimInstance* AnimInstance = Cast<UMilitiaAnimInstance>(ServiceOwner->GetMesh()->GetAnimInstance()))
							AnimInstance->HasMoveOrder = true;
						//停止当前的导航移动
						AIController->StopMovement();
					}

					/*--------------*/
					//如果HasMoveOrder为true，计算何时让Unit停止向目标点移动（置HasMoveOrder为空）
					/*--------------*/
					if (BlackBoard->GetValueAsBool(BlackBoardKey_HasMoveOrder.SelectedKeyName)) {
						//计算距离目标位置的距离
						FVector CurrentUnitLocation = ServiceOwner->GetActorLocation();
						FVector DistanceToTargetLocation = TargetLocationToMove - CurrentUnitLocation;
						float Distance = DistanceToTargetLocation.Size();
						if (Distance <= 100) {
							BlackBoard->SetValueAsBool(BlackBoardKey_HasMoveOrder.SelectedKeyName, false);
							//动画蓝图状态更新
							if (UMilitiaAnimInstance* AnimInstance = Cast<UMilitiaAnimInstance>(ServiceOwner->GetMesh()->GetAnimInstance())) {
								AnimInstance->HasMoveOrder = false;
							}
							UE_LOG(LogTemp, Log, TEXT("Task:到达目标位置附近（0）黑板值已设置false"));
						}

					}
					
				}

				
				

			}
		}
	}


				
}