// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_Death.h"
#include "Character/Core/RuleOfCharacter.h"

// 仅在编辑器模式下包含预览角色头文件
#if WITH_EDITOR
#include "AnimationEditorPreviewActor.h" // 修正路径并移除"Persona/Public/"
#endif

class AAnimationEditorPreviewActor;

UAnimNotify_Death::UAnimNotify_Death()
	:Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(1, 1, 1, 255);
#endif //WITH_EDITORONLY_DATA
}

void UAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
#if WITH_EDITOR
	//此处用到了Persona模块
	//若用编辑器启动，则进行转换判断：若转换成功则在游戏中运行，跳过。从而给“确保Anim预览在实例被Destroy后仍能正常运行”加双层保险
	AAnimationEditorPreviewActor* PreviewCharacter = Cast<AAnimationEditorPreviewActor>(MeshComp->GetOuter());
	if (!PreviewCharacter) {
		if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {
			Character->Destroy();
			Character->OnActorDeath.Broadcast();
		}
	}
#else
	if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {
		Character->Destroy();
		Character->OnActorDeath.Broadcast();
	}
#endif
}
