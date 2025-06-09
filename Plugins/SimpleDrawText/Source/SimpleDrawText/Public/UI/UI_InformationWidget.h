// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_InformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEDRAWTEXT_API UUI_InformationWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextWidget;
	
public:
	/*
	** @Param InText:伤害  Percentage:伤害与目标生命比值，若>1,则字体放大表明暴击
	*/
	void SetTextBlockStyle(const FString& InText, FLinearColor Color, float Percentage);
};
