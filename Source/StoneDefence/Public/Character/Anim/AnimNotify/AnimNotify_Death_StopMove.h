// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Death_StopMove.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UAnimNotify_Death_StopMove : public UAnimNotify
{
	GENERATED_BODY()

	UAnimNotify_Death_StopMove();

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
