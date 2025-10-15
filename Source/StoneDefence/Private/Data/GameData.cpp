#include "Data/GameData.h"

FGameInstanceDatas::FGameInstanceDatas() {
	Init();
}

void FGameInstanceDatas::Init() {
	//RemainNumberOfMonster = 20;
	GameDifficulty = 0;
	bAllMainTowersDied = false;
	bGameOver = false;
	bTimeFreezed = false;
	bCurrentLevelMissionSuccess = false;
	MaxNumberOfMonster = 4;
	CurrentLevel = INDEX_NONE;
	SpawnTimeInterval = 1.f;
	//CurrentSpawnMonsterTime = 0.0f;
	MaxStage = 4;
	GameTimeCount = 90;
	MaxGameTimeCount = 0;
	GlodGrowthTime = 1.f;
	MaxGlodGrowthTime = 3.0f;
	KilledMobNumber = 0;
	KilledBossNumber = 0;
	TotalDiedTower = 0;
	TotalDiedMainTower = 0;
}

bool FGameInstanceDatas::IsValid() {
	return false;
}

float FGameInstanceDatas::GetPercentageOfRemainMob() {
	if (MaxNumberOfMonster) {
		return (float)GetRemainMobNumbers() / (float)MaxNumberOfMonster;
	}
	return 0;
}

void FGameInstanceDatas::ResetCurrentSpawn() {
	CurrentSpawnMonsterTime = 0.0f;
}

void FGameInstanceDatas::AssignedMonsterAmount() {
	//UE_LOG(LogTemp, Error, TEXT("fuck"));
	int32 CurrentMobNumber = MaxNumberOfMonster;
	int32 CurrentStageNumber = MaxStage;
	int32 CurrentStageAssignedMobNum = 0;
	if (CurrentMobNumber > 1) {
		for (int32 i = 0; i < CurrentStageNumber; i++) {
			float EverageMobNumInEachStage = (float)MaxNumberOfMonster / (float)CurrentStageNumber;
			CurrentStageNumber--;
			if (CurrentStageNumber > 1) {//若不是最后一波
				CurrentStageAssignedMobNum = FMath::RandRange(EverageMobNumInEachStage / 6, EverageMobNumInEachStage);
		 	}
			else {//若为最后一波
				CurrentStageAssignedMobNum = EverageMobNumInEachStage;
			}
			MobNumberinCurrentStage.Add(CurrentStageAssignedMobNum);		
			//RemainNumberOfMonster -= CurrentStageAssignedMobNum;        RemainNumberOfMonster已移除，需重构
		}
	}
	else {
		MobNumberinCurrentStage.Add(CurrentStageAssignedMobNum);

	}

}

void FGameInstanceDatas::StageDecision() {
	int32 CurrentStage = MobNumberinCurrentStage.Num() - 1;
	if (MobNumberinCurrentStage.Num()) {
		if (MobNumberinCurrentStage[CurrentStage] > 0) {
			MobNumberinCurrentStage[CurrentStage]--;
		}
		else {
			MobNumberinCurrentStage.RemoveAt(CurrentStage);
		}
	}
	else {
		bCurrentLevelMissionSuccess = true;
	}
}


int32 FGameInstanceDatas::GetRemainMobNumbers() {
	return 0;
}