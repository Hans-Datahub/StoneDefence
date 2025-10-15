// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArchiveDelegate.h"

UArchiveDelegate* UArchiveDelegate::Instance = nullptr;

UArchiveDelegate* UArchiveDelegate::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UArchiveDelegate>();
        Instance->AddToRoot(); // 防止被垃圾回收
    }
    return Instance;
}

void UArchiveDelegate::Initialize()
{
    // 确保实例被创建
    GetInstance();
}

void UArchiveDelegate::Shutdown()
{
    if (Instance)
    {
        Instance->RemoveFromRoot();
        Instance = nullptr;
    }
}