// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectLevelUI/Core/TD_SelectLevelHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/SelectLevelUI/UMG/UI_SelectLevelMain.h"

ATD_SelectLevelHUD::ATD_SelectLevelHUD() {
	//按照TEXT中所给的路径寻找蓝图类，并赋值给SelectMain_BP class
	static ConstructorHelpers::FClassFinder<UUI_SelectLevelMain>SelectLevelMain_BPClass(TEXT("/Game/UI/Select/SelectMain_BP"));
	//找到后再次赋值
	SelectLevelMainClass = SelectLevelMain_BPClass.Class;
}

void ATD_SelectLevelHUD::BeginPlay() {
	Super::BeginPlay();
	//创建实例
	SelectLevelMain = CreateWidget<UUI_SelectLevelMain>(GetWorld(), SelectLevelMainClass);
	SelectLevelMain->AddToViewport();
}
