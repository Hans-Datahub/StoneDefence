// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RuleOfAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API URuleOfAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	URuleOfAnimInstance();

	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimAttrubute")
	bool IsDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimAttrubute")
	bool HasAttackOrder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimAttrubute")
	float Speed;

private:
	float CurrentTime;//用于延迟生成怪物以避免生成出来的怪物直接死亡
	bool isDelayFinished;

	bool IsDelayFinished(float Deltaseconds);
};
