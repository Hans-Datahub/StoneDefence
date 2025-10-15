// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_GameInstance.h"
#include "SimpleScreenLoading.h"
#include "Core/RuleOfTheGameState.h"
#include "Kismet/GameplayStatics.h"
#include "../StoneDefenceMacro.h"
#include "../StoneDefenceUtils.h"
#include "Core/RuleOfThePlayerState.h"


#define LOCTEXT_NAMESPACE "TowerGameInstance"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

UTD_GameInstance::UTD_GameInstance()
	: ISimpleArchivesInterface()
	, SaveSlotNumber(INDEX_NONE)
	, GameSaveType(EGameSaveType::NOSAVE)
{

}

void UTD_GameInstance::Init() {
	Super::Init();

	FSimpleScreenLoadingModule& SimpleScreenLoadingModule = FModuleManager::LoadModuleChecked<FSimpleScreenLoadingModule>("SimpleScreenLoading");
	SimpleScreenLoadingModule.SetupScreenLoading();
}

int32 UTD_GameInstance::GetSaveSlotNumber() const
{
	int32 SlotNumber = 20;
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		SlotNumber = InGameState->GetSaveSlotList().Slots.Num();
	}

	return SlotNumber;
}

FSaveSlotList* UTD_GameInstance::GetSlotList()
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		return &InGameState->GetSaveSlotList();
	}

	return nullptr;
}

bool UTD_GameInstance::OpenLevel(int32 SaveNumber)
{
	if (FSaveSlot* InSlot = GetSaveSlot(SaveNumber))
	{
		if (InSlot->bSave)
		{
			//存储我们的存档是第几个
			SaveSlotNumber = SaveNumber;
			GameSaveType = EGameSaveType::ARCHIVES;
			OpenLevelOnServer(InSlot->LevelName);
			return true;
		}
	}

	return false;
}

bool UTD_GameInstance::IsSlotValid(int32 SaveNumber) const
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		if (FSaveSlot* InSlot = InGameState->GetSaveSlot(SaveNumber))
		{
			return InSlot->bSave;
		}
	}

	return false;
}

FSaveSlot* UTD_GameInstance::GetSaveSlot(int32 SaveNumber)
{
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		return InGameState->GetSaveSlot(SaveNumber);
	}

	return nullptr;
}

UWorld* UTD_GameInstance::GetSimpleWorld() const
{
	return GetWorld();
}

bool UTD_GameInstance::SaveGameData(int32 SaveNumber)
{
	bool bSave = false;

	//游戏存储
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		if (FSaveSlot* InSlot = InGameState->GetSaveSlot(SaveNumber))
		{
			InSlot->DateText = FText::FromString(FDateTime::Now().ToString());
			InSlot->LevelName = LOCTEXT("LevelName", "TestMap");
			InSlot->ChapterName = LOCTEXT("ChapterName", "Hello World~~");
			InSlot->GameThumbnail.ReleaseResources();
			InSlot->GameThumbnail.ScrPath = GAMETHUMBNAIL_SCREENSHOT(
				400, 200,
				InSlot->GameThumbnail.GameThumbnail,
				GetWorld())->GetFilename();

			bSave = InGameState->SaveGameData(SaveNumber);


			//更新SaveList，用于主菜单存档页面数据调用
			FSaveSlotList& InSlotList = InGameState->GetSaveSlotList();
			InSlotList.Slots.Add(SaveNumber, *InSlot);
			InSlotList.DegreeOfCompletion.Add(SaveNumber, 0);
		}

		
	}

	//玩家数据存储
	StoneDefenceUtils::CallUpdateAllBaseClient(GetSafeWorld(), [&](APlayerController* InPlayerController)
		{
			if (ARuleOfThePlayerState* InState = InPlayerController->GetPlayerState<ARuleOfThePlayerState>())
			{
				bSave = InState->SaveGameData(SaveNumber);
			}
		});

	return bSave;
}

bool UTD_GameInstance::ClearGameData(int32 SaveNumber)
{
	bool bSave = false;
	if (ARuleOfTheGameState* InGameState = GetGameState())
	{
		bSave = InGameState->ClearGameData(SaveNumber);
	}

	StoneDefenceUtils::CallUpdateAllBaseClient(GetSafeWorld(), [&](APlayerController* InPlayerController)
		{
			if (ARuleOfThePlayerState* InState = InPlayerController->GetPlayerState<ARuleOfThePlayerState>())
			{
				bSave = InState->ClearPlayerData(SaveNumber);
			}
		});

	return bSave;
}

bool UTD_GameInstance::ReadGameData(int32 SaveNumber)
{
	return false;
}

void UTD_GameInstance::SetSaveNumber(int32 SaveNumber)
{
	SetCurrentSaveSlotNumber(SaveNumber);
}

ARuleOfTheGameState* UTD_GameInstance::GetGameState() const
{
	/*if (UWorld* World = GetSafeWorld()) {
		return World->GetGameState<ARuleOfTheGameState>();
	}
	return nullptr;*/


	//return  GetSafeWorld() == nullptr ? nullptr :  GetSafeWorld()->GetGameState<ARuleOfTheGameState>();


	UWorld* World = GetSafeWorld();

	// 增强检查：不仅要检查指针，还要检查对象状态
	if (World == nullptr || !IsValid(World) || World->IsUnreachable())
	{
		UE_LOG(LogTemp, Warning, TEXT("World is invalid or pending kill"));
		return nullptr;
	}

	// 检查World是否正在被销毁
	if (World->bIsTearingDown)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is tearing down"));
		return nullptr;
	}

	return World->GetGameState<ARuleOfTheGameState>();
}

UWorld* UTD_GameInstance::GetSafeWorld() const
{
	if (!GetWorld())
	{
		return GEngine->GetWorld();
	}

	return GetWorld();
}

int32 UTD_GameInstance::GetCurrentSaveSlotNumber() const
{
	return SaveSlotNumber;
}

void UTD_GameInstance::ClearSaveMark()
{
	SaveSlotNumber = INDEX_NONE;
	GameSaveType = EGameSaveType::NOSAVE;
}

void UTD_GameInstance::SetCurrentSaveSlotNumber(int32 InSaveSlotNumber)
{
	SaveSlotNumber = InSaveSlotNumber;
}

void UTD_GameInstance::OpenLevelOnServer(const FText& MapName)
{
	UGameplayStatics::OpenLevel(GetSafeWorld(), *MapName.ToString());
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

#undef LOCTEXT_NAMESPACE