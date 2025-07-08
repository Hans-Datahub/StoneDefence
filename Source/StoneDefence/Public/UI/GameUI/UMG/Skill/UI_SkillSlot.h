// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameUI/UMG/Core/UI_Slot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI_SkillSlot.generated.h"


class UImage;
class Ubutton;
class UTextBlock;

UCLASS()
class STONEDEFENCE_API UUI_SkillSlot : public UUI_Slot
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerSkillNumber;// = Number 对应技能按键

public:
	virtual void NativeConstruct();

	FORCEINLINE int32 GetKeyNumberForSKill() const { return KeyNumberForSKill; };
private:
	int32 KeyNumberForSKill;// = KeyNumber
};
