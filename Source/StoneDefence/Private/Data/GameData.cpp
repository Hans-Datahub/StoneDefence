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
	CurrentSpawnMilitaTime = 0.0f;
	MaxStage = 4;
	GameTimeCount = 90;
	MaxGameTimeCount = 0;
	GlodGrowthTime = 1.f;
	MaxGlodGrowthTime = 3.0f;
	KilledMobNumber = 0;
	KilledBossNumber = 0;
	TotalDiedTower = 0;
	TotalDiedMainTower = 0;

	//-----------------Lowpoly Part Parameters-----------------------//
	RemainNumberOfMilitia = 20;
	MaxNumberOfMilitia = 4;
	CurrentSpawnMilitaTime = 0.0f;
	//MilitiaNumberinCurrentStage
	KilledMilitiabNumber = 0;

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
	CurrentSpawnMilitaTime = 0.0f;
}

void FGameInstanceDatas::AssignedMonsterAmount() {
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

void FGameInstanceDatas::AssignedMilitiaAmount() {
	int32 CurrentMilitiaNumber = MaxNumberOfMilitia;
	int32 CurrentStageNumber = MaxStage;
	int32 CurrentStageAssignedMilitiaNum = 0;
	if (CurrentMilitiaNumber > 1) {
		for (int32 i = 0; i < CurrentStageNumber; i++) {
			float EverageMilitiaNumInEachStage = (float)MaxNumberOfMilitia / (float)CurrentStageNumber;
			CurrentStageNumber--;
			if (CurrentStageNumber > 1) {//若不是最后一波
				CurrentStageAssignedMilitiaNum = FMath::RandRange(EverageMilitiaNumInEachStage / 6, EverageMilitiaNumInEachStage);
			}
			else {//若为最后一波
				CurrentStageAssignedMilitiaNum = EverageMilitiaNumInEachStage;
			}
			MilitiaNumberinCurrentStage.Add(CurrentStageAssignedMilitiaNum);
		}
	}
	else {
		MilitiaNumberinCurrentStage.Add(CurrentStageAssignedMilitiaNum);

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

void FGameInstanceDatas::MilitiaStageDecision() {
	int32 CurrentStage = MilitiaNumberinCurrentStage.Num() - 1;
	if (MilitiaNumberinCurrentStage.Num()) {
		if (MilitiaNumberinCurrentStage[CurrentStage] > 0) {
			MilitiaNumberinCurrentStage[CurrentStage]--;
		}
		else {
			MilitiaNumberinCurrentStage.RemoveAt(CurrentStage);
		}
	}
	else {
		bCurrentLevelMissionSuccess = true;
	}
}


int32 FGameInstanceDatas::GetRemainMobNumbers() {
	return 0;
}