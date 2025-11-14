// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Militia.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifyQueue.h"

void AMilitia::BeginDestroy()
{
    Super::BeginDestroy();

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (MeshComp)
    {
        // 停止动画播放，防止新的通知产生
        MeshComp->Stop();

        // 清空动画通知队列
        if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
        {
            ClearAnimNotifyQueue(AnimInstance);
        }

        // 解除动画实例关联（可选，进一步防止访问）
        MeshComp->SetAnimInstanceClass(nullptr);
    }
}

void AMilitia::ClearAnimNotifyQueue(UAnimInstance* AnimInstance)
{
    if (!AnimInstance || !AnimInstance->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Warning, TEXT("ClearAnimNotifyQueue: AnimInstance is null or invalid"));
        return;
    }
    
    // 1. 清空所有待处理的动画通知
    AnimInstance->NotifyQueue.AnimNotifies.Empty();

    // 2. 清空蒙太奇未过滤的通知
    AnimInstance->NotifyQueue.UnfilteredMontageAnimNotifies.Empty();

    // 3. 清理活跃的通知状态（避免残留的 NotifyState 回调）
    AnimInstance->ActiveAnimNotifyState.Empty();
    AnimInstance->ActiveAnimNotifyEventReference.Empty();

    // 4. 停止所有蒙太奇，防止新通知产生
    AnimInstance->StopAllMontages(0.0f);
}


ETeam AMilitia::GetTeamType() {
	return ETeam::RED;
}

void AMilitia::RegisterTeam() {
	GetCharacterData().Team == ETeam::RED;
}