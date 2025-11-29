// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/LowPolyGameMode.h"
#include "Core/GameCore/LowPolyGameState.h"
#include "Core/GameCore/LowPolyPlayerController.h"
#include "Selection/SelectionManager.h"
#include "Item/SpawnPoint.h"
#include "UI/GameUI/Core/RuleOfHUD.h"
#include "../StoneDefenceUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Core/GameCore/TD_GameInstance.h"
#include "UI_ArchivesSystem.h"



ALowPolyGameMode::ALowPolyGameMode() {
	GameStateClass = ALowPolyGameState::StaticClass();
	SelectionManagerClass = ASelectionManager::StaticClass();
	PlayerControllerClass = ALowPolyPlayerController::StaticClass();
}

void ALowPolyGameMode::BeginPlay() {
	Super::BeginPlay();

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


	//角色数据还原
	UTD_GameInstance* InGameInstance = GetWorld()->GetGameInstance<UTD_GameInstance>();
	if (!InGameInstance)return;
	ALowPolyGameState* InGameState = GetGameState<ALowPolyGameState>();
	if (!InGameState)return;


	if (InGameInstance->GetCurrentSaveSlotNumber() == INDEX_NONE &&
		InGameInstance->GetGameType() == EGameSaveType::NOSAVE)//开启新游戏
	{
		
		//从表中读取标准关卡数据
		InGameState->GetGameData().InitLevelData(InGameState, false);
		//根据读取数据初始化
		InitStandardData();

	}
	else {//通过存档读取的数据	

					

		//读取关卡数据
		InGameState->GetGameData().InitLevelData(InGameState, true);
		//根据读取数据初始化
		InitDataFormArchives();
		
		//还原我们场景中的角色
		ARuleOfCharacter* Unit;
		TMap<FGuid, FCharacterData> DataSet = InGameState->GetSaveData()->CharacterDatas;		
		for (auto& DataPair : DataSet){
			FCharacterData Data = DataPair.Value;
			if (Data.Team == ETeam::RED) {
				Unit = SpawnMilitia(Data, Data.Location, Data.Rotator);

			}
			else {
				Unit = SpawnMarine(Data, Data.Location, Data.Rotator);

			}
		}

		InGameInstance->ClearSaveMark();//清 除存档痕迹
	}
}

void ALowPolyGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	//若开启全新游戏，而非从存档读取，则进行完整的全新单位刷新
	if (UTD_GameInstance* InGameInstance = GetWorld()->GetGameInstance<UTD_GameInstance>())
		if (InGameInstance->GetCurrentSaveSlotNumber() == INDEX_NONE &&	
			InGameInstance->GetGameType() == EGameSaveType::NOSAVE)
			UpdateUnitSpawnRule(DeltaSeconds);
}



void ALowPolyGameMode::InitStandardData() {
	if (ALowPolyGameState* InGameState = GetGameState<ALowPolyGameState>())
	{
		InGameState->GetGameData().AssignedMilitiaAmount();
		InGameState->GetGameData().AssignedMarineAmount();
	}
}



AMilitia* ALowPolyGameMode::SpawnMilitia(
	const FCharacterData& CharacterData,
	const FVector& Location,
	const FRotator& Rotator,
	int32 CharacterLevel) {
	return Cast<AMilitia>(ATD_GameMode::SpawnCharacterWithData(
		CharacterData,
		Location,
		Rotator,
		CharacterLevel));
}

AMarine* ALowPolyGameMode::SpawnMarine(
	const FCharacterData& CharacterData,
	const FVector& Location,
	const FRotator& Rotator,
	int32 CharacterLevel) {
	return Cast<AMarine>(ATD_GameMode::SpawnCharacterWithData(
		CharacterData,
		Location,
		Rotator,
		CharacterLevel));
}


AMilitia* ALowPolyGameMode::SpawnStandardMilitiaFromTable(
	int32 CharacterID,
	const FVector& Location,
	const FRotator& Rotator,
	int32 CharacterLevel) {
	return ATD_GameMode::SpawnStandardCharacterFromTable<AMilitia>(
		CharacterID,
		GetGameState<ALowPolyGameState>()->AIMilitiaCharacterData,
		Location,
		Rotator,
		CharacterLevel);
}

AMarine* ALowPolyGameMode::SpawnStandardMarineFromTable(
	int32 CharacterID,
	const FVector& Location,
	const FRotator& Rotator,
	int32 CharacterLevel) {
	return ATD_GameMode::SpawnStandardCharacterFromTable<AMarine>(
		CharacterID,
		GetGameState<ALowPolyGameState>()->AIMarineCharacterData,
		Location,
		Rotator,
		CharacterLevel);
}



void ALowPolyGameMode::UpdateUnitSpawnRule(float DeltaSeconds) {
	if (ATD_GameState* TempGameState = GetGameState<ALowPolyGameState>()) {
		FGameInstanceDatas& GameData = TempGameState->GetGameData();
		if (!GameData.bCurrentLevelMissionSuccess) {                   /*当前关卡是否胜利*/
			if (!GameData.bGameOver) {                                //游戏是否结束
				if (GameData.MilitiaNumberinCurrentStage.Num()) {    //是否还有波数
					GameData.CurrentSpawnMilitaTime += DeltaSeconds;//生成间隔计时累加
					if (GameData.bAllowSpawnMilitia()) {           //是否经过固定生成间隔
						GameData.ResetCurrentSpawn();             //重置计时

						//出生点归类汇总
						TArray<ASpawnPoint*> MilitiaSpawnPoints, MarineSpawnPoints;
						for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
							if (ETeam::RED == SpawnPoint->Team)
								MilitiaSpawnPoints.Add(SpawnPoint);
							else if (ETeam::BLUE == SpawnPoint->Team)
								MarineSpawnPoints.Add(SpawnPoint);
						}

						//单位生成
						int32 MobDifficulty = 1;// GetTowerDifficultyParam_Level(GetWorld());
						if (MilitiaSpawnPoints.Num()) {//若有出生点
							if (GameData.MilitiaCurrentStage >= 0) {
								//若还有人数剩余，则生成
								if (GameData.MilitiaNumberinCurrentStage[GameData.MilitiaCurrentStage]) {
									if (AMilitia* Militia = SpawnStandardMilitiaFromTable(-1, FVector::ZeroVector, FRotator::ZeroRotator, MobDifficulty)) {
										ASpawnPoint* OneOfSpawnPoint = MilitiaSpawnPoints[FMath::RandRange(0, MilitiaSpawnPoints.Num() - 1)];
										Militia->SetActorLocationAndRotation(OneOfSpawnPoint->GetActorLocation(), OneOfSpawnPoint->GetActorRotation());
									}
								}//若当前波数人耗尽
								else {
									
									if (GameData.TimeCountForSpawnGap <= GameData.GapForSpawn)
										GameData.TimeCountForSpawnGap += DeltaSeconds;
									else {//当间隔时间结束
										GameData.TimeCountForSpawnGap = 0.f;
										//进行下一波生成
										if (GameData.MilitiaCurrentStage > 0)
											GameData.MilitiaCurrentStage--;
									}

									


								}
							}
							
							//更新波数与每波人数，全部消灭后置Success为true
							GameData.MilitiaStageDecision();
						}
						
						if (MarineSpawnPoints.Num()) {
							if (GameData.MarineCurrentStage >= 0) {
								if (GameData.MarineNumberinCurrentStage[GameData.MarineCurrentStage]) {
									if (AMarine* Marine = SpawnStandardMarineFromTable(-1, FVector::ZeroVector, FRotator::ZeroRotator, MobDifficulty)) {
										ASpawnPoint* OneOfSpawnPoint = MarineSpawnPoints[FMath::RandRange(0, MarineSpawnPoints.Num() - 1)];
										Marine->SetActorLocationAndRotation(OneOfSpawnPoint->GetActorLocation(), OneOfSpawnPoint->GetActorRotation());
									}
								}
							}						
							GameData.MarineStageDecision();
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