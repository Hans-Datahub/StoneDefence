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

	UPROPERTY(meta = (BindWidget))
	UImage* TowerIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* TowerCD;

	//Towers Prepare Building Number
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TPBNumber;

	//Towers Completion Of Construction Number
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TCCNumber;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TowersCDValue;

	//Towers Inventory Slot Button
	UPROPERTY(meta = (BindWidget))
	UButton* TISButton;

	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicCDMaterial;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName TowersCDMaterialName;

	//若CD完成后不清除会有一条线
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName TowersCDClearName;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_IconDragDrop> IconDragDropClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_TowerTip> TowerTipClass;

public:
	virtual void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void OnClickedWidget();

	FBuildingTowers& GetBuildingTower();

	void UpdateUI();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	void ClearSlotAfterFinishedDrag();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
	UWidget* GetTowerTip();

protected:
	virtual FReply NativeOnMouseButtonDown (const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

private:
	void UpdateTowersCD(float InDeltaTime);
	void DrawTowersCD(float InTowersCD);
	void DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber);
	void UpdateTowersBuildingInfo();

};
