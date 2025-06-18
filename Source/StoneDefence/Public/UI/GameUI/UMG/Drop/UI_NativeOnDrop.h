// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_NativeOnDrop.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_NativeOnDrop : public UUI_RuleOfWidget
{
	GENERATED_BODY()
	
public:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

};
