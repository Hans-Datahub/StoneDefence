// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_Death_StopMove.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Core/GameCore/LowPolyGameState.h"
#include "AnimationEditorPreviewActor.h"
#include "Character/Anim/MilitiaAnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


UAnimNotify_Death_StopMove::UAnimNotify_Death_StopMove() 
	:Super()
{

}

void UAnimNotify_Death_StopMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	AAnimationEditorPreviewActor* PreviewCharacter = Cast<AAnimationEditorPreviewActor>(MeshComp->GetOuter());
	if (!PreviewCharacter) {
		if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {
			AAIController* AIController = Cast<AAIController>(Character->GetController());
			if (!AIController) {
				UE_LOG(LogTemp, Error, TEXT("UAnimNotify_DeathStopMove: Can't find AIController to clean the BT"));
				return;
			}
			UBehaviorTreeComponent* BTC = AIController->FindComponentByClass<UBehaviorTreeComponent>();
			if (!BTC) {
				UE_LOG(LogTemp, Error, TEXT("UAnimNotify_DeathStopMove: Can't find BTC to clear"));
				return;
			}
			BTC->StopTree();      // 终止行为树运行（中断所有任务）
			UE_LOG(LogTemp, Log, TEXT("BT stopped for AIController: %s"), *AIController->GetName());
			//行为树都停了，更改HasMoveOrder似乎也是无所谓了
			Cast<UMilitiaAnimInstance>(Character->GetMesh()->GetAnimInstance())->HasMoveOrder = false;
		}
	}
}