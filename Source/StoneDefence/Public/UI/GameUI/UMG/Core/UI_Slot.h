// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UI_Slot.generated.h"



class UImage;
class UButton;
class UTextBlock;

UCLASS()
class STONEDEFENCE_API UUI_Slot : public UUI_RuleOfWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		UImage* SlotIcon;

	UPROPERTY(meta = (BindWidget))
		UImage* SlotImageCD;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SlotCDValue;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SlotNumber;//在InventorySlot中表示已构建数量，在SkillSlot中表示 。。。

	UPROPERTY(meta = (BindWidget))
		UButton* ClickButton;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		FName SlotCDMaterialName;

	//若CD完成后不清除会有一条线
	UPROPERTY(EditDefaultsOnly, Category = UI)
		FName SlotCDClearName;

	UPROPERTY()
		class UMaterialInstanceDynamic* DynamicCDMaterial;

	bool bNeedTick = false;


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	virtual void NativeConstruct();
	virtual void ClearSlotAfterFinishedDrag();

	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget() {};

	void UpdateSlotUI(UTexture2D* Icon, int32 ConstructionNumber);	
	void DrawSlotCD(float InTowersCD);
	void DisplaySlotNumber(UTextBlock* TextNumberBlock, int32 TextNumber);
	void UpdateSlotInfo(int32 ConstructionNumber, float InSlotCD);
	
};
