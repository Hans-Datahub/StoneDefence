// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "RuleOfDamage.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API URuleOfDamage : public UDamageType
{
	GENERATED_BODY()

public:
		const class FSkillData* SkillData = nullptr;
};
