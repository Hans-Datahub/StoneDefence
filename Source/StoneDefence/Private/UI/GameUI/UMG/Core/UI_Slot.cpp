// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Core/UI_Slot.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUI_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	ClickButton->OnClicked.AddDynamic(this, &UUI_Slot::OnClickedWidget);

	DynamicCDMaterial = SlotImageCD->GetDynamicMaterial();
}

void UUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_Slot::ClearSlotAfterFinishedDrag()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
	SlotImageCD->SetVisibility(ESlateVisibility::Hidden);
	SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
	SlotNumber->SetVisibility(ESlateVisibility::Hidden);
} 

void UUI_Slot::UpdateSlotInfo(int32 ConstructionNumber, float InSlotCD)
{
	DisplaySlotNumber(SlotCDValue, InSlotCD);
	DisplaySlotNumber(SlotNumber, ConstructionNumber);
}

void UUI_Slot::UpdateSlotUI(UTexture2D* Icon, int32 ConstructionNumber)
{
	if (Icon)
	{	
		SlotIcon->SetBrushFromTexture(nullptr);
		SlotIcon->SetBrushFromTexture(Icon);
		SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SlotIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	if (ConstructionNumber > 0)
	{
		SlotNumber->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_Slot::DrawSlotCD(float InSlotCD)
{
	if (DynamicCDMaterial)
	{
		if (InSlotCD > 0.0f && InSlotCD < 1.0f)
		{
			DynamicCDMaterial->SetScalarParameterValue(SlotCDClearName, true);
			SlotImageCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			DynamicCDMaterial->SetScalarParameterValue(SlotCDClearName, false);
			SlotImageCD->SetVisibility(ESlateVisibility::Hidden);
		}

		DynamicCDMaterial->SetScalarParameterValue(TEXT("Percentage"), InSlotCD);
	}
}


void UUI_Slot::DisplaySlotNumber(UTextBlock* TextNumberBlock, int32 TextNumber)
{
	//TextNumberBlock->SetText(FText::GetEmpty()); //清除文本内容，防止上帧内容残留
	if (TextNumber < 1)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TextNumber)));
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

