// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/WrapBox.h"
#include "UI/GameUI/UMG/Skill/UI_TakenSkillSlot.h"
#include "UI_Health.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_Health : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Title;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Health;
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* TakenSkillList;
	TSubclassOf<class UUI_TakenSkillSlot>TakenSkillSlotClass;

public:
	virtual void NativeConstruct() override;
	void SetTitle(const FString& Msg);
	void SetHealth(float HealthValue);
	void AddTakenSkillSlot(const FGuid &TakenSkillID);
	bool RemoveTakenSkillSlot(const FGuid &TakenSkillID);
};
