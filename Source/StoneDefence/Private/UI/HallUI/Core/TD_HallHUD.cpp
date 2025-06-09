// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/Core/TD_HallHUD.h"
#include "UI/HallUI/UMG/UI_MainHall.h"
#include "UObject/ConstructorHelpers.h"

ATD_HallHUD::ATD_HallHUD() {
	//����TEXT��������·��Ѱ����ͼ�࣬����ֵ��HallMain_BPClass
	static ConstructorHelpers::FClassFinder<UUI_MainHall>HallMain_BPClass(TEXT("/Game/UI/Hall/HallMain_BP"));
	//�ҵ����ٴθ�ֵ
	MainHallClass = HallMain_BPClass.Class;
}

void ATD_HallHUD::BeginPlay() {
	 
	Super::BeginPlay();
	// ���� UUI_MainHall �ؼ���ʵ��
	MainHall = CreateWidget<UUI_MainHall>(GetWorld(), MainHallClass);
	MainHall->AddToViewport();

}