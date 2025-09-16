// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/UI_GameSettings.h"
#include "Widget/InputKeySelectorMapping.h"
#include "GameFramework/InputSettings.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UUI_GameSettings::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateInputKey();
}

void UUI_GameSettings::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_GameSettings::SaveSettings()
{

}

void UUI_GameSettings::LoadSettings()
{

}

void UUI_GameSettings::ChangeInputKey(FInputChord SelectedKey)
{
	if (KeySelectorTmp)
	{
		TArray<FInputActionKeyMapping> OutMappings;
		UInputSettings::GetInputSettings()->GetActionMappingByName(KeySelectorTmp->ActionName, OutMappings);
		if (OutMappings.Num())
		{
			UInputSettings::GetInputSettings()->RemoveActionMapping(OutMappings[0]);
			FInputActionKeyMapping InputActionKeyMapping;
			InputActionKeyMapping.ActionName = KeySelectorTmp->ActionName;
			InputActionKeyMapping.Key = KeySelectorTmp->SelectedKey.Key;
			InputActionKeyMapping.bShift = KeySelectorTmp->SelectedKey.bShift;
			InputActionKeyMapping.bCmd = KeySelectorTmp->SelectedKey.bCmd;
			InputActionKeyMapping.bCtrl = KeySelectorTmp->SelectedKey.bCtrl;
			InputActionKeyMapping.bAlt = KeySelectorTmp->SelectedKey.bAlt;

			UInputSettings::GetInputSettings()->AddActionMapping(InputActionKeyMapping);
			UInputSettings::GetInputSettings()->SaveKeyMappings();
		}
	}
}

void UUI_GameSettings::OnSelectingKeyChanged(UInputKeySelectorMapping* InInputKey)
{
	KeySelectorTmp = InInputKey;
}

void UUI_GameSettings::UpdateInputKey()
{
	TArray<FName> ActionNames;
	UInputSettings::GetInputSettings()->GetActionNames(ActionNames);
	for (auto& Tmp : ActionNames)
	{
		if (UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass()))
		{
			if (UTextBlock* NewText = NewObject<UTextBlock>(this, UTextBlock::StaticClass(), Tmp, RF_Transactional))
			{
				NewText->SetText(FText::FromName(Tmp));
				UHorizontalBoxSlot* BoxSlotA = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(NewText));
				BoxSlotA->SetSize(ESlateSizeRule::Fill);

				TArray<FInputActionKeyMapping> OutMappings;
				UInputSettings::GetInputSettings()->GetActionMappingByName(Tmp, OutMappings);
				if (OutMappings.Num())
				{
					if (UInputKeySelectorMapping* KeySelector = NewObject<UInputKeySelectorMapping>(this, UInputKeySelectorMapping::StaticClass(), *FGuid::NewGuid().ToString(), RF_Transactional))
					{
						KeySelector->OnKeySelected.AddDynamic(this, &UUI_GameSettings::ChangeInputKey);
						KeySelector->OnIsSelectingKeyChanged.AddDynamic(this, &UUI_GameSettings::OnSelectingKeyChanged);

						FInputChord InSelectedKey;
						InSelectedKey.Key = OutMappings[0].Key;
						InSelectedKey.bShift = OutMappings[0].bShift;
						InSelectedKey.bCmd = OutMappings[0].bCmd;
						InSelectedKey.bCtrl = OutMappings[0].bCtrl;
						InSelectedKey.bAlt = OutMappings[0].bAlt;
						KeySelector->ActionName = OutMappings[0].ActionName;
						KeySelector->SetSelectedKey(InSelectedKey);

						UHorizontalBoxSlot* BoxSlotB = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(KeySelector));
						BoxSlotB->SetSize(ESlateSizeRule::Fill);
					}
				}


				ActionList->AddChild(HorizontalBox);
			}
		}
	}
}
