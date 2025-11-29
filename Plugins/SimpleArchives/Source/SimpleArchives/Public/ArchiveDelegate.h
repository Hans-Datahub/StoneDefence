// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h" // 对于非UObject的代理声明
#include "Delegates/Delegate.h" 
#include "ArchiveDelegate.generated.h"

// 声明存档系统关闭的多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArchivesClosedDelegate);
// 存档序号变更委托（参数：新序号）
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArchiveIndexChanged, int32, NewSlotIndex);

UCLASS()
class SIMPLEARCHIVES_API UArchiveDelegate : public UObject
{
    GENERATED_BODY()

public:
    // 获取代理管理器单例（全局访问点）
    static UArchiveDelegate* GetInstance();

    // 存档系统关闭事件代理
    UPROPERTY(BlueprintAssignable)
        FOnArchivesClosedDelegate OnArchivesClosed;

    // 序号变更事件
    UPROPERTY(BlueprintAssignable)
        FOnArchiveIndexChanged OnArchiveIndexChanged;

    // 初始化单例实例
    static void Initialize();
    // 清理单例实例
    static void Shutdown();

    UFUNCTION(BlueprintCallable, Category = "Archives")
     int32 GetCurrentSlotIndex() const;

private:
    static UArchiveDelegate* Instance;

};