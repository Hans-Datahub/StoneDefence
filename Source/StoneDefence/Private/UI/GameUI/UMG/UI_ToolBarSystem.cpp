// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/UI_ToolBarSystem.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUI_ToolBarSystem::NativeConstruct(){
	Super::NativeConstruct();

}

void UUI_ToolBarSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	/*GetGPlayerState()->GetPlayerData();
	GetGameState()->GetGameData();*/
	//ToolBar数据更新
	GameTimeCount->SetText(FText::FromString(GetCurrentTimeCount(GetGameState()->GetGameData().GameTimeCount)));
	Coins->SetText(FText::AsNumber(GetPlayerState()->GetPlayerData().GameGold));
	TowerDeathNumber->SetText(FText::AsNumber(GetGameState()->GetGameData().TotalDiedTower));
	KilledSoldier->SetText(FText::AsNumber(GetGameState()->GetGameData().KilledMobNumber));
	SurplusQuantity->SetText(FText(FText::FromString(FString::Printf(TEXT("%02d/%02d"),
														GetGameState()->GetGameData().MobNumberinCurrentStage.Num(),
																		GetGameState()->GetGameData().MaxStage - GetGameState()->GetGameData().MobNumberinCurrentStage.Num()))));
	SurplusQuantityProgressBar->SetPercent(GetGameState()->GetGameData().GetPercentageOfRemainMob());
}

FString UUI_ToolBarSystem::GetCurrentTimeCount(float CounttingTime) {
	const int32 Time = FMath::Max(0, FMath::TruncToInt(CounttingTime));
	const int32 Minutes = Time / 60;
	const int32 Seconds = Time % 60;
	return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
