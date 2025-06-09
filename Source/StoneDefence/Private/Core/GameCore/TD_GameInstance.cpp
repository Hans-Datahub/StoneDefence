// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_GameInstance.h"
#include "SimpleScreenLoading.h"

void UTD_GameInstance::Init() {
	Super::Init();

	FSimpleScreenLoadingModule& SimpleScreenLoadingModule = FModuleManager::LoadModuleChecked<FSimpleScreenLoadingModule>("SimpleScreenLoading");
	SimpleScreenLoadingModule.SetupScreenLoading();
} 