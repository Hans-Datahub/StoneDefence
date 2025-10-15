// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_TakenSkillTip.generated.h"

UCLASS()
class STONEDEFENCE_API UUI_TakenSkillTip : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* TakenSkillTipIntroduction;

};