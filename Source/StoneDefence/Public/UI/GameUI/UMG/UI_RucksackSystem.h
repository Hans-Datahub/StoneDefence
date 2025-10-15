// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameUI/UMG/Drop/UI_NativeOnDrop.h"
#include "UI_RucksackSystem.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_RucksackSystem : public UUI_NativeOnDrop
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UUI_Inventory* Inventory;

	virtual void NativeConstruct();
	void UpdateInventorySlot(const FGuid& InventorySlotGUID, bool bInCD);

};
