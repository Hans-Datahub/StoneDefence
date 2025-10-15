// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SettingDelegate.h"

USettingDelegate* USettingDelegate::Instance = nullptr;

USettingDelegate* USettingDelegate::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<USettingDelegate>();
        Instance->AddToRoot(); // 防止被垃圾回收
    }
    return Instance;
}

void USettingDelegate::Initialize()
{
    // 确保实例被创建
    GetInstance();
}

void USettingDelegate::Shutdown()
{
    if (Instance)
    {
        Instance->RemoveFromRoot();
        Instance = nullptr;
    }
}