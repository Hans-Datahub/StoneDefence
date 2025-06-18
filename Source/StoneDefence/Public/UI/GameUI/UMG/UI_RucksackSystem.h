// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_RucksackSystem.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_RucksackSystem : public UUI_RuleOfWidget
{
	GENERATED_BODY()

		UPROPERTY(meta = (BindWidget))
		class UUI_Inventory* Inventory;

public:
	virtual void NativeConstruct();
	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);
};
