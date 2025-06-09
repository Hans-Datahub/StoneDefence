// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_Death.h"
#include "Persona/Public/AnimationEditorPreviewActor.h"
#include "Character/Core/RuleOfCharacter.h"

class AAnimationEditorPreviewActor;

UAnimNotify_Death::UAnimNotify_Death()
	:Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(0, 0, 0, 255);
#endif //WITH_EDITORONLY_DATA
}

void UAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
#if WITH_EDITOR
	//�˴��õ���Personaģ��
	//���ñ༭�������������ת���жϣ���ת���ɹ�������Ϸ�����У��������Ӷ�����ȷ��AnimԤ����ʵ����Destroy�������������С���˫�㱣��
	AAnimationEditorPreviewActor* Character = Cast<AAnimationEditorPreviewActor>(MeshComp->GetOuter());
	if (!Character) {
		if (ARuleOfCharacter* CharacterActor = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {
			CharacterActor->Destroy();
			CharacterActor->OnActorDeath.Broadcast();
		}
	}
#else
	if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {
		Character->Destroy();
		CharacterActor->OnActorDeath.Broadcast();
	}
#endif
}
