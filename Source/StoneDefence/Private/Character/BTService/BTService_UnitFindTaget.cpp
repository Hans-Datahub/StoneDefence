// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BTService/BTService_UnitFindTaget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/AIController/MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Anim/RuleOfAnimInstance.h"
#include "Character/CharacterCore/Marine.h"
#include "Character/Anim/MilitiaAnimInstance.h"


void UBTService_UnitFindTaget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, Nodememory, DeltaSeconds);



	if (AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetOwner())) {
		//对于Marine来说, 若有移动命令则停止攻击有关指令, 直接结束本TickNode (由于动画蓝图未做区分，所以用的都是MilitiaAnim)
		if (AMarine* UnitAI = Cast<AMarine>(AIController->GetPawn()))
			if (UMilitiaAnimInstance* AnimInstance = Cast<UMilitiaAnimInstance>(UnitAI->GetMesh()->GetAnimInstance()))
				if (AnimInstance->HasMoveOrder == true) {
					UnitAI->Isattack = false;
					return;
				}


		if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent()) {
			if (ARuleOfCharacter* NewTarget = Cast<ARuleOfCharacter>(AIController->FindTarget())) {
				if (!NewTarget->IsDeath()) {
					//---若新搜索到的目标为新目标---//
					if (AIController->Target != NewTarget) {
						if (ARuleOfCharacter* UnitSelf = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
							//当搜寻到新目标停止当前活动，否则会优先执行完当前任务才会开始新活动
							UnitSelf->GetCharacterMovement()->StopMovementImmediately();

							//停止当前攻击动画（蒙太奇）
							//UMilitiaAnimInstance* TempAnimInstance = Cast<UMilitiaAnimInstance>(UnitSelf->GetMesh()->GetAnimInstance());
							//TempAnimInstance->InitializeAnimation();
						}
						//更换目标
						AIController->Target = NewTarget;
					}

					//---若目标没变，则计算带不包括攻击范围的距离---//
					if (AIController->Target.IsValid()) {
						if (AIController->Target->IsActive()) {
							////计算怪与塔之间距离（向量）
							FVector TargetLocation = AIController->Target.Get()->GetActorLocation();
							FVector SelfLocation = AIController->GetPawn()->GetActorLocation();

							// 计算从目标指向自身的单位方向
							FVector DirectionToSelf = (SelfLocation - TargetLocation).GetSafeNormal();

							// 从目标位置反向偏移300单位
							FVector LocationWithGap = TargetLocation + (DirectionToSelf * 2000.0f);

							MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, AIController->Target.Get());//Get()用于将弱指针所指的Target转化成对象实例
							MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, LocationWithGap);//此处由LocationWithGap更改为TargetLocation
						}
						else {
							MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
							MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
						}
					}
					else {
						MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
						MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
					}
				}


			}//如果没搜到新的敌人
			else {
				MyBlackBoard->SetValueAsObject(BlackBoardKey_Target.SelectedKeyName, NULL);
				MyBlackBoard->SetValueAsVector(BlackBoardKey_TargetLocation.SelectedKeyName, FVector::ZeroVector);
				//暂停攻击
				//if (ARuleOfCharacter* TempUnit = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
				//	TempUnit->Isattack = false;

				//	//停止攻击动画（蒙太奇）
				//	UMilitiaAnimInstance* TempAnimInstance = Cast<UMilitiaAnimInstance>(TempUnit->GetMesh()->GetAnimInstance());
				//	TempAnimInstance->InitializeAnimation();
				//}
			}

			if (ARuleOfCharacter* UnitAI = Cast<ARuleOfCharacter>(AIController->GetPawn())) {
				if (AIController->Target.IsValid()) {
					if (!AIController->Target->IsDeath()) {
						FVector Mylocation = AIController->GetPawn()->GetActorLocation();
						FVector TMDistance = Mylocation - AIController->Target->GetActorLocation();

						/*if (TMDistance.Size() > 2200) { UnitAI->Isattack = false; }
						else { UnitAI->Isattack = true; }*/

						MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, TMDistance.Size());
					}
					else//若目标死亡
						UnitAI->Isattack = false;
				}
				else {
					MyBlackBoard->SetValueAsFloat(BlackBoardKey_Distance.SelectedKeyName, 0.0f);
					UnitAI->Isattack = false;
				}


				//判断攻击状态是否变化
				//if (bOldIsattack != UnitAI->Isattack) {
				//	bOldIsattack = UnitAI->Isattack;
				//	//若变化为停止攻击，停止蒙太奇播放
				//	if (bOldIsattack == false)
				//		if (URuleOfAnimInstance* AnimInstance = Cast<URuleOfAnimInstance>(UnitAI->GetMesh()->GetAnimInstance()))
				//			if(UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
				//				AnimInstance->Montage_Stop(0.1f, CurrentMontage);
				//}
			}


			//临时设置最终目标位置  测试
			MyBlackBoard->SetValueAsVector(BlackBoardKey_FinalTargetLocation.SelectedKeyName, FVector(-7000,-7620,0));
		}
	}
}