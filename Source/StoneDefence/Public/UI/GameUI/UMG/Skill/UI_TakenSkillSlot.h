// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Data/Core/SkillData.h"
#include "UI_TakenSkillSlot.generated.h"

//本类在教程中原名：“UICharacterSkillSlot”
UCLASS()
class STONEDEFENCE_API UUI_TakenSkillSlot : public UUI_RuleOfWidget {
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UBorder* TakenSkillBackGround;

	UPROPERTY(meta = (BindWidget))
		UImage* TakenSkillCD;

	UPROPERTY()
		class UMaterialInstanceDynamic* DynamicCDMaterial;

public:
	void NativeConstruct()override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	FSkillData& GetSkillData();

	void DrawTakenSkillCD(float TakenSkillCD);

	void SetTexture(UTexture2D* InTexture);
};