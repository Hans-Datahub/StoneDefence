// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/Core/TD_HallHUD.h"
#include "UI/HallUI/UMG/UI_MainHall.h"
#include "UObject/ConstructorHelpers.h"

ATD_HallHUD::ATD_HallHUD() {
	//按照TEXT中所给的路径寻找蓝图类，并赋值给HallMain_BPClass
	static ConstructorHelpers::FClassFinder<UUI_MainHall>HallMain_BPClass(TEXT("/Game/UI/Hall/HallMain_BP"));
	//找到后再次赋值
	MainHallClass = HallMain_BPClass.Class;
}

void ATD_HallHUD::BeginPlay() {
	 
	Super::BeginPlay();
	// 创建 UUI_MainHall 控件的实例
	MainHall = CreateWidget<UUI_MainHall>(GetWorld(), MainHallClass);
	MainHall->AddToViewport();

}