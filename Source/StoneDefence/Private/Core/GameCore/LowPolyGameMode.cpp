// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/LowPolyGameMode.h"
#include "Core/GameCore/LowPolyGameState.h"
#include "Core/GameCore/LowPolyPlayerController.h"
#include "Selection/SelectionManager.h"
#include "Item/SpawnPoint.h"
#include "UI/GameUI/Core/RuleOfHUD.h"
#include "../StoneDefenceUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


ALowPolyGameMode::ALowPolyGameMode() {
	GameStateClass = ALowPolyGameState::StaticClass();
	SelectionManagerClass = ASelectionManager::StaticClass();
	PlayerControllerClass = ALowPolyPlayerController::StaticClass();
}

void ALowPolyGameMode::BeginPlay() {
	if (ALowPolyGameState* TempGameState = GetGameState<ALowPolyGameState>())
		TempGameState->GetGameData().AssignedMilitiaAmount();//分配每波怪物数量

	//SelectionManager的单例生成
	if (SelectionManagerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SelectionManager = GetWorld()->SpawnActor<ASelectionManager>(
			SelectionManagerClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
	}
}


void ALowPolyGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	UpdateMilitiaSpawnRule(DeltaSeconds);
}


AMilitia* ALowPolyGameMode::SpawnMilitia(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator) {
	return SpawnCharacter<AMilitia>(CharacterID, CharacterLevel, GetGameState<ALowPolyGameState>()->AIMilitiaCharacterData, Location, Rotator);
}

AMarine* ALowPolyGameMode::SpawnMarine(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator) {
	return SpawnCharacter<AMarine>(CharacterID, CharacterLevel, GetGameState<ALowPolyGameState>()->AIMarineCharacterData, Location, Rotator);
}

void ALowPolyGameMode::UpdateMilitiaSpawnRule(float DeltaSeconds) {
	if (ATD_GameState* TempGameState = GetGameState<ALowPolyGameState>()) {
		if (!TempGameState->GetGameData().bCurrentLevelMissionSuccess) {                   /*当前关卡是否胜利*/
			if (!TempGameState->GetGameData().bGameOver) {                                //游戏是否结束
				if (TempGameState->GetGameData().MilitiaNumberinCurrentStage.Num()) {    //是否还有波数
					TempGameState->GetGameData().CurrentSpawnMilitaTime += DeltaSeconds;//生成间隔计时累加
					if (TempGameState->GetGameData().bAllowSpawnMilitia()) {           //是否经过固定生成间隔
						TempGameState->GetGameData().ResetCurrentSpawn();             //重置计时

						//出生点归类汇总
						TArray<ASpawnPoint*> MilitiaSpawnPoints, MarineSpawnPoints;
						for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
							if (ETeam::RED == SpawnPoint->Team)
								MilitiaSpawnPoints.Add(SpawnPoint);
							else if (ETeam::RED == SpawnPoint->Team)
								MarineSpawnPoints.Add(SpawnPoint);
						}


						int32 MobDifficulty = 1;// GetTowerDifficultyParam_Level(GetWorld());
						if (MilitiaSpawnPoints.Num()) {//若有出生点
							if (AMilitia* Militia = SpawnMilitia(-1, MobDifficulty, FVector::ZeroVector, FRotator::ZeroRotator)) {
								ASpawnPoint* OneOfSpawnPoint = MilitiaSpawnPoints[FMath::RandRange(0, MilitiaSpawnPoints.Num() - 1)];
								Militia->SetActorLocationAndRotation(OneOfSpawnPoint->GetActorLocation(), OneOfSpawnPoint->GetActorRotation());


								TempGameState->GetGameData().MilitiaStageDecision();//------------------
							}
						}
						if (MarineSpawnPoints.Num()) {
							//Marine生成……
						}


					}
				}
			}
		}
		else {
			/*播放胜利画面*/
		}
	}
}