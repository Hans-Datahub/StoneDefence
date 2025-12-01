// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArchivesScreenshotTypes.h"  // 添加这行，包含委托定义


DECLARE_DELEGATE_OneParam(FSimpleArchivesSlotDelegate,FSimpleDelegate)

enum class EArchivesState :uint8
{
	NONE,
	SAVE,
	LOAD,
};

////新的委托类型 ，用于插件中截图功能与主项目中的通信
//UENUM(BlueprintType)
//enum class EArchivesState : uint8
//{
//	LOAD UMETA(DisplayName = "Load"),
//	SAVE UMETA(DisplayName = "Save")
//};