// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_NativeOnDrop.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_NativeOnDrop : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

};
