// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_ToolBarSystem.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUI_ToolBarSystem::NativeConstruct(){
	Super::NativeConstruct();

}

void UUI_ToolBarSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	//ToolBar数据更新

	//倒计时
	GameTimeCount->SetText(FText::FromString(GetCurrentTimeCount(GetGameState()->GetGameData().GameTimeCount)));

	//金币
	int32 CoinsAmount = GetPlayerState()->GetPlayerData().GameGold;
	FText CoinsText = FText::Format(FText::FromString("Coins:{0}"), FText::AsNumber(CoinsAmount));
	Coins->SetText(CoinsText);

	//Marine死亡数
	int32 MarineDeathNum = GetGameState()->GetGameData().TotalDiedMarine;
	FText MarineDeathText = FText::Format(FText::FromString("Marine DeathNum:{0}"), FText::AsNumber(MarineDeathNum));
	TowerDeathNumber->SetText(MarineDeathText);

	//Militia死亡数
	int32 MilitiaDeathNum = GetGameState()->GetGameData().TotalDiedMilitia;
	FText MilitiaDeathText = FText::Format(FText::FromString("Militia DeathNum:{0}"), FText::AsNumber(MilitiaDeathNum));
	KilledSoldier->SetText(MilitiaDeathText);

	//关卡进度
	FText CurrentLevelText = FText::AsNumber(GetGameState()->GetGameData().MilitiaCurrentStage + 1);
	FText MaxStageText = FText::AsNumber(GetGameState()->GetGameData().MaxStage);
	FText SurplusQuantityText = FText::Format(FText::FromString("Current/Total(Level):{0}/{1}"), CurrentLevelText, MaxStageText);
	SurplusQuantity->SetText(SurplusQuantityText);

	//关卡进度（进度条）
	//SurplusQuantityProgressBar->SetPercent(GetGameState()->GetGameData().GetPercentageOfRemainMilitia());
	SurplusQuantityProgressBar->SetPercent(0.5f);
	SurplusQuantityProgressBar->SetVisibility(ESlateVisibility::Visible);
}

FString UUI_ToolBarSystem::GetCurrentTimeCount(float CounttingTime) {
	const int32 Time = FMath::Max(0, FMath::TruncToInt(CounttingTime));
	const int32 Minutes = Time / 60;
	const int32 Seconds = Time % 60;
	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
