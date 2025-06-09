// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/Core/RuleOfCharacter.h"
#include "RuleOfAIController.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ARuleOfAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds)override;
	virtual AActor* FindTarget() { return nullptr; };
 	virtual void AttackTarget(class ARuleOfCharacter* AttackTarget);
	//����ָ�����,��Ŀ�����٣���ָ���Զ��ÿ�
	TWeakObjectPtr<class ARuleOfCharacter> Target;
};
	