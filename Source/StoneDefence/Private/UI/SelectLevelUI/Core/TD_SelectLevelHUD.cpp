// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectLevelUI/Core/TD_SelectLevelHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/SelectLevelUI/UMG/UI_SelectLevelMain.h"

ATD_SelectLevelHUD::ATD_SelectLevelHUD() {
	//����TEXT��������·��Ѱ����ͼ�࣬����ֵ��SelectMain_BP class
	static ConstructorHelpers::FClassFinder<UUI_SelectLevelMain>SelectLevelMain_BPClass(TEXT("/Game/UI/Select/SelectMain_BP"));
	//�ҵ����ٴθ�ֵ
	SelectLevelMainClass = SelectLevelMain_BPClass.Class;
}

void ATD_SelectLevelHUD::BeginPlay() {
	Super::BeginPlay();
	//����ʵ��
	SelectLevelMain = CreateWidget<UUI_SelectLevelMain>(GetWorld(), SelectLevelMainClass);
	SelectLevelMain->AddToViewport();
}
