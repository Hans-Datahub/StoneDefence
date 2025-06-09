// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_PlayerSkillSystem.generated.h"

struct FKey;//�̳���Ϊclass�������ڱ���ĳ���struct
class UUI_SkillSlot;

UCLASS()
class STONEDEFENCE_API UUI_PlayerSkillSystem : public UUI_RuleOfWidget
{
	GENERATED_BODY()



public:
	virtual void NativeConstruct();

//private:
	FKey FreezeSkillKey;
	FKey ExplosionSkillKey;
	FKey RecoverySkillKey;
	FKey RecoveryMainTowerSkillKey;
};
