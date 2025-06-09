// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_IconDragDrop.generated.h"

class UImage;

UCLASS()
class STONEDEFENCE_API UUI_IconDragDrop : public UUI_RuleOfWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

public:
	void SetDragIcon(UTexture2D* ICON);
};
