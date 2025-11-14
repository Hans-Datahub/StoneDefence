// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_Death.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Core/GameCore/LowPolyGameState.h"

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

	//为防止角色销毁之后，队列中的动画通知仍在访问动画实例，优先进行检查
	// 检查 MeshComp 是否有效
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UAnimNotify_Death: MeshComp is null"));
		return;
	}
	// 检查动画实例是否有效
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance || !AnimInstance->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("UAnimNotify_Death: AnimInstance is null or invalid"));
		return;
	}



	//此处用到了Persona模块
	//若用编辑器启动，则进行转换判断：若转换成功则在游戏中运行，跳过。从而给“确保Anim预览在实例被Destroy后仍能正常运行”加双层保险
	AAnimationEditorPreviewActor* PreviewCharacter = Cast<AAnimationEditorPreviewActor>(MeshComp->GetOuter());
	if (!PreviewCharacter) {
		if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {			
			GetWorld()->GetGameState<ALowPolyGameState>()->RemoveCharacterData(Character->GUID);			
			Character->OnActorDeath.Broadcast();

			USkeletalMeshComponent* WeaponMeshComponent = GetWeapon(MeshComp);
			WeaponMeshComponent->DestroyComponent();
			Character->Destroy();
		}
	}
#else
	if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter())) {		
		GetWorld()->GetGameState<ALowPolyGameState>()->RemoveCharacterData(Character->GUID);
		Character->OnActorDeath.Broadcast();
		Character->Destroy();
	}
#endif	
}

USkeletalMeshComponent* UAnimNotify_Death::GetWeapon(USkeletalMeshComponent* MeshComp) {
	//获取武器Mesh，进而获取OpenFireSocket
	ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter());
	USkeletalMeshComponent* WeaponMesh = nullptr;
	FName WeaponSocketName = "WeaponSlot_Index"; // 你在蓝图中设置的Socket名称

	if (Character->GetMesh()->DoesSocketExist(WeaponSocketName))
	{
		// 获取附加到Socket的Actor
		TArray<AActor*> AttachedActors;
		Character->GetAttachedActors(AttachedActors);

		for (AActor* AttachedActor : AttachedActors) {
			if (USkeletalMeshComponent* FoundMesh = AttachedActor->FindComponentByClass<USkeletalMeshComponent>())
			{
				WeaponMesh = FoundMesh;
				break; // 找到后退出循环
			}
		}
	}
	return WeaponMesh;
}