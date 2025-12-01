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

bool UTD_GameInstance::SaveGameData(int32 SlotIndex, FArchivesScreenshotComplete OnScreenshotDone)
{
	bool bSave = false;

	ARuleOfTheGameState* InGameState = GetGameState();
	if (!InGameState) return bSave;
	TWeakObjectPtr<ARuleOfTheGameState> WeakGameState = InGameState; // 弱引用，检测GameState是否存活 

	//1. 游戏数据存储（GameData）
	bSave = InGameState->SaveGameData(SlotIndex);

	// 2. 获取存档槽引用
	/*FSaveSlotList& TempSlotList = WeakGameState->GetSaveSlotList();
	if (TempSlotList.Slots.Num() == 0) return bSave;
	FSaveSlotList SlotListCopy = TempSlotList;*/

	FSaveSlotList& RawSlotList = WeakGameState->GetSaveSlotList();
	if (RawSlotList.Slots.Num() == 0) return bSave;

	// 3. 创建主项目的截图回调
	RenderingUtils::FOnScreenshotComplete InternalCallback;

	// 捕获插件的回调和必要数据
	InternalCallback.BindLambda([this, SlotIndex, OnScreenshotDone, WeakGameState, &RawSlotList](UTexture2D* GeneratedTexture) mutable
		{
			//FSaveSlot& SaveSlot = RawSlotList.Slots[SlotIndex];
			FSaveSlot& SaveSlot = RawSlotList.Slots[SlotIndex];

			// 3.1 保存其他游戏数据
			SaveSlot.DateText = FText::FromString(FDateTime::Now().ToString());
			SaveSlot.LevelName = FText::FromString(GetWorld()->GetMapName().Replace(*GetWorld()->StreamingLevelsPrefix, TEXT("")));
			SaveSlot.ChapterName = LOCTEXT("ChapterName", "Hello World~~");		
			SaveSlot.bSave = true;

			// 3.2使用带回调的截图
			
			/*SaveSlot.GameThumbnail.ScrPath = GAMETHUMBNAIL_SCREENSHOT_WITH_CALLBACK(
				400, 200,
				SaveSlot.GameThumbnail.GameThumbnail,
				GetWorld(),
				OnScreenshotDone)->GetFilename();*/
			SaveSlot.GameThumbnail.GameThumbnail = GeneratedTexture;
			SaveSlot.GameThumbnail.ScrPath = FPaths::ProjectSavedDir() / TEXT("SaveGames") / GeneratedTexture->GetName() + TEXT(".jpg");

			// 3.3 更新SaveList，用于主菜单存档页面数据调用
			RawSlotList.Slots.Add(SlotIndex, SaveSlot);
			RawSlotList.DegreeOfCompletion.Add(SlotIndex, 0);

			// 3.4 保存到磁盘
			// UGameplayStatics::SaveGameToSlot(...);

			// ============================================================
			// 关键：调用插件传入的回调，通知UI更新
			// ============================================================
			if (OnScreenshotDone.IsBound())
			{
				OnScreenshotDone.Execute(GeneratedTexture);
			}

			//SaveSlot.GameThumbnail.ReleaseResources();


		});

	// 4. 发起截图请求
	// 校验
	//UTexture2D* TargetTexture = nullptr;
	//auto TempSlotSet = RawSlotList.Slots;
	//if (IsValid(RawSlotList.Slots[SlotIndex].GameThumbnail.GameThumbnail))
	//	TargetTexture = RawSlotList.Slots[SlotIndex].GameThumbnail.GameThumbnail;
	UTexture2D*& TargetTexture = RawSlotList.Slots[SlotIndex].GameThumbnail.GameThumbnail; // 取原始纹理的引用
	
	// 使用主项目的 RenderingUtils::FScreenShot
	RenderingUtils::FScreenShot* ScreenshotInstance = new RenderingUtils::FScreenShot(
		400,                           // 宽度
		200,                           // 高度
		TargetTexture,   // 输出纹理引用
		this,                          // Outer
		80,                            // JPEG质量
		false,                         // 不显示UI
		false,                          // 添加文件名后缀
		InternalCallback               // 完成回调
	);
	//在上面的InternalCallback绑定的Lambda中无法获取外部的ScreenshotInstance里的FileName，所以在这儿获取
	//RawSlotList.Slots[SlotIndex].GameThumbnail.ScrPath = ScreenshotInstance->GetFilename();

	// 5. 玩家数据存储（PlayerData）
	StoneDefenceUtils::CallUpdateAllBaseClient(GetSafeWorld(), [&](APlayerController* InPlayerController)
		{
			if (ARuleOfThePlayerState* InState = InPlayerController->GetPlayerState<ARuleOfThePlayerState>())
			{
				bSave = InState->SaveGameData(SlotIndex);
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