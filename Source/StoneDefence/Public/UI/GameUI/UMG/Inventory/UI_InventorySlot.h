// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameUI/UMG/Core/UI_Slot.h"
#include "Components/Button.h"
#include "Data/Save/GameSaveData.h"
#include "Blueprint/UserWidget.h"
#include "UI_InventorySlot.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class STONEDEFENCE_API UUI_InventorySlot : public UUI_Slot
{
	GENERATED_BODY()
public:

	//Towers Prepare Building Number
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TPBNumber;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_IconDragDrop> IconDragDropClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_TowerTip> TowerTipClass;

	virtual void NativeConstruct();

	virtual void OnClickedWidget() override;

	FBuildingTowers& GetBuildingTower();

	void UpdateUI();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	void ClearSlotAfterFinishedDrag();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
	UWidget* GetTowerTip();

	void UpdateTowersBuildingInfo();

protected:
	virtual FReply NativeOnMouseButtonDown (const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	void UpdateTowersCD(float InDeltaTime);

};
