// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_ArchivesSystem.h"
#include "Core/SimpleArchivesGlobalVariable.h"
#include "Histroy/UI_ArchivesBar.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"


class UCheckBox;

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

void UUI_ArchivesSystem::InitArchivesSystem(EArchivesState ArchivesState)
{
	switch (ArchivesState)
	{
	case EArchivesState::LOAD:
		SaveGameButton->SetIsEnabled(false);
		break;
	case EArchivesState::SAVE:
		LoadGameButton->SetIsEnabled(false);
		break;
	}
}

void UUI_ArchivesSystem::LoadGame()
{
	ISimpleArchivesInterface* MyArchives = GetCurrentArchivesInterface();
	if (!MyArchives) return;

	TArray<UUI_ArchivesBar*> InArchivesBars;
	if (!GetArchivesBarArray(InArchivesBars)) return;

	int BarIndex = 0;
	for (UUI_ArchivesBar* OneOfBar : InArchivesBars){
		if (OneOfBar->CheckBoxButton->IsChecked())
			break;
		BarIndex++;
	}

	CurrentSaveSlot = BarIndex;
	SimpleSlotIndex = BarIndex;
	MyArchives->OpenLevel(SimpleSlotIndex);
}

void UUI_ArchivesSystem::SaveGame()
{
	if (SimpleSlotIndex == INDEX_NONE) return;

	ISimpleArchivesInterface* ArchInterface = GetCurrentArchivesInterface();
	if (!ArchInterface)return;

	// 创建截图完成回调
	// 注意：我们需要捕获 SimpleSlotIndex 的当前值，因为它可能在回调执行前改变
	const int32 CapturedSlotIndex = SimpleSlotIndex;

	// 绑定截图完成回调
	FArchivesScreenshotComplete OnScreenshotDone;
	OnScreenshotDone.BindLambda([this, CapturedSlotIndex, ArchInterface](UTexture2D* NewTexture) {
		
		// 1. 获取存档接口（需要重新获取，因为这是异步回调）
		ISimpleArchivesInterface* CallbackArchInterface = GetCurrentArchivesInterface();
		if (!CallbackArchInterface) return;

		// 2. 获取对应的存档槽数据
		FSaveSlot* InSlot = ArchInterface->GetSaveSlot(CapturedSlotIndex);
		if (!InSlot) return;

		// 3. 更新UI显示
		// 遍历所有存档条目，找到对应槽位并更新其显示
		CallAllArchivesBarBreak([this, CapturedSlotIndex, InSlot, NewTexture](UUI_ArchivesBar* Tmp) -> bool
			{
				if (Tmp->SlotIndex == CapturedSlotIndex)
				{
					ResetArchivesBar(Tmp, InSlot);
					Tmp->SetGameThumbnail(NewTexture);
					return true;
				}

				return false;
			});

	});
	// 调用带回调的SaveGameData
	ArchInterface->SaveGameData(CapturedSlotIndex, OnScreenshotDone);
	
}

void UUI_ArchivesSystem::CloseArchivesSystem() {

	RemoveFromParent();

	// 通知所有监听者
	BroadcastCloseEvent(); // 发布者广播事件
}

void UUI_ArchivesSystem::NativeConstruct()
{
	Super::NativeConstruct();

	LoadGameButton->OnClicked.AddDynamic(this, &UUI_ArchivesSystem::LoadGame);
	SaveGameButton->OnClicked.AddDynamic(this, &UUI_ArchivesSystem::SaveGame);
	CloseButton->OnClicked.AddDynamic(this, &UUI_ArchivesSystem::CloseArchivesSystem);
	UpdateArchivesSlot();
}

void UUI_ArchivesSystem::NativeDestruct()
{
	Super::NativeDestruct();

	SimpleSlotIndex = INDEX_NONE;
}

void UUI_ArchivesSystem::BindWindows(TFunction<void(FSimpleDelegate)> NewWindows)
{
	TArray<UUI_ArchivesBar*> InArchivesBars;
	if (GetArchivesBarArray(InArchivesBars))
	{
		for (auto &Tmp : InArchivesBars)
		{
			Tmp->CallNewWindowsDelegate = FSimpleArchivesSlotDelegate::CreateLambda(NewWindows);
		}
	}
}

bool UUI_ArchivesSystem::GetArchivesBarArray(TArray<UUI_ArchivesBar*> &InArchivesBars)
{
	for (UPanelSlot *PanelSlot : SaveSlotList->GetSlots())
	{
		if (UUI_ArchivesBar *ArchivesSlot = Cast<UUI_ArchivesBar>(PanelSlot->Content))
		{
			InArchivesBars.Add(ArchivesSlot);
		}
	}

	return InArchivesBars.Num() > 0;
}

void UUI_ArchivesSystem::UpdateArchivesSlot()
{
	if (ArchivesBarClass)
	{
		if (ISimpleArchivesInterface * MyArchives = GetCurrentArchivesInterface())
		{
			if (FSaveSlotList *InSlotList = MyArchives->GetSlotList())
			{		
				for (int32 i = 0; i < InSlotList->Slots.Num(); i++)
				{
					if (UUI_ArchivesBar *UIArchivesBar = CreateWidget<UUI_ArchivesBar>(GetWorld(), ArchivesBarClass))
					{
						SaveSlotList->AddChild(UIArchivesBar);
						UIArchivesBar->SlotIndex = i;

						//反向代理
						UIArchivesBar->ReverseProxy = FSimpleDelegate::CreateUObject(this, &UUI_ArchivesSystem::CallAllCkeckBox, UIArchivesBar);
				
						UIArchivesBar->Update();

						//还原记录的数据
						if (InSlotList->Slots[i].bSave)
						{
							ResetArchivesBar(UIArchivesBar,&InSlotList->Slots[i]);
						}
					}
				}
			}
		}
	}
}

void UUI_ArchivesSystem::CallAllCkeckBox(UUI_ArchivesBar* OwnerArchivesBar)
{
	CallAllArchivesBar([&](UUI_ArchivesBar *Tmp)
	{
		if (Tmp != OwnerArchivesBar)
		{
			Tmp->SetCheckBoxState(ECheckBoxState::Unchecked);
		}		
	});
}

void UUI_ArchivesSystem::CallAllArchivesBar(TFunction<void(UUI_ArchivesBar* InArchivesBar)> InArchivesBarFunc)
{
	TArray<UUI_ArchivesBar*> InArchivesBars;
	if (GetArchivesBarArray(InArchivesBars))
	{
		for (auto &Tmp : InArchivesBars)
		{
			InArchivesBarFunc(Tmp);
		}
	}
}

void UUI_ArchivesSystem::CallAllArchivesBarBreak(TFunction<bool (UUI_ArchivesBar* InArchivesBar)> InArchivesBarFunc)
{
	TArray<UUI_ArchivesBar*> InArchivesBars;
	if (GetArchivesBarArray(InArchivesBars))
	{
		for (auto &Tmp : InArchivesBars)
		{
			if (InArchivesBarFunc(Tmp))
			{
				break;
			}
		}
	}
}

void UUI_ArchivesSystem::ResetArchivesBar(UUI_ArchivesBar* InArchivesBar, const FSaveSlot *InData)
{
	if (InArchivesBar && InData)
	{
		InArchivesBar->SetSaveGameDate(InData->DateText);
		InArchivesBar->SetChapterName(InData->ChapterName);
		
	}
}



									//-------------代理部分-----------//

void UUI_ArchivesSystem::BroadcastCloseEvent()
{
	//当关闭时广播事件
	UArchiveDelegate* Delegates = UArchiveDelegate::GetInstance();
	if (Delegates)
		Delegates->OnArchivesClosed.Broadcast();
}


#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif