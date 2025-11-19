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
	CurrentLevel = 1;
	SpawnTimeInterval = 1.f;
	//CurrentSpawnMonsterTime = 0.0f;
	CurrentSpawnMilitaTime = 0.0f;
	MaxStage = 1;
	GameTimeCount = 180;
	MaxGameTimeCount = 0;
	GlodGrowthTime = 1.f;
	MaxGlodGrowthTime = 3.0f;
	KilledMobNumber = 0;
	KilledBossNumber = 0;
	TotalDiedTower = 0;
	TotalDiedMainTower = 0;

	//-----------------Lowpoly Part Parameters-----------------------//
	//RemainNumberOfMilitia = 20;
	MaxNumberOfMilitia = 1;
	MaxNumberOfMarine = 1;
	CurrentSpawnMilitaTime = 0.0f;
	//MilitiaNumberinCurrentStage
	KilledMilitiabNumber = 0;

	TotalDiedMarine = 0;
	TotalDiedMilitia = 0;
	TimeCountForSpawnGap = 0.f;
	GapForSpawn = 10.0f;
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

float FGameInstanceDatas::GetPercentageOfRemainMilitia() {
	if (MaxNumberOfMilitia) {
		return (float)GetRemainMilitiaNumbers() / (float)MaxNumberOfMilitia;
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
	int32 CurrentStageAssignedMilitiaNum = 0; 
	if (CurrentMilitiaNumber > 1) {
		for (int32 CurStage = 0; CurStage < MaxStage; CurStage++) {
			float EverageMilitiaNumInEachStage = (float)MaxNumberOfMilitia / (float)MaxStage;
			
			if (CurStage < MaxStage - 1) {//若不是最后一波
				//随机分配数量，并为剩余数量去掉本次分配的部分
				CurrentStageAssignedMilitiaNum = (int32)FMath::RandRange(EverageMilitiaNumInEachStage / 6, EverageMilitiaNumInEachStage);
				CurrentMilitiaNumber -= CurrentStageAssignedMilitiaNum;
			}
			else//若为最后一波，分配剩余所有单位
				CurrentStageAssignedMilitiaNum = CurrentMilitiaNumber;

			//将确定的数量加入数组
			MilitiaNumberinCurrentStage.Add(CurrentStageAssignedMilitiaNum);
		}
	}
	else//若只剩1人
		MilitiaNumberinCurrentStage.Add(1);

	//初始化当前波数
	MilitiaCurrentStage = MilitiaNumberinCurrentStage.Num() - 1;
}

void FGameInstanceDatas::AssignedMarineAmount() {	
	//Marine直接一次性部署
	MarineNumberinCurrentStage.Add(MaxNumberOfMarine);
	MarineCurrentStage = MarineNumberinCurrentStage.Num() - 1;

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
	if (MilitiaNumberinCurrentStage.Num()) {
		if (MilitiaNumberinCurrentStage[MilitiaCurrentStage] > 0) {
			MilitiaNumberinCurrentStage[MilitiaCurrentStage]--;
		}
		else {
			MilitiaNumberinCurrentStage.RemoveAt(MilitiaCurrentStage);
			MilitiaCurrentStage--;
		}
	}
	else {
		bCurrentLevelMissionSuccess = true;
	}
}

void FGameInstanceDatas::MarineStageDecision() {
	if (MarineNumberinCurrentStage.Num()) {
		if (MarineNumberinCurrentStage[MarineCurrentStage] > 0) {
			MarineNumberinCurrentStage[MarineCurrentStage]--;
		}
		else {
			MarineNumberinCurrentStage.RemoveAt(MarineCurrentStage);
			MarineCurrentStage--;
		}
	}
}


int32 FGameInstanceDatas::GetRemainMobNumbers() {
	return 0;
}

int32 FGameInstanceDatas::GetRemainMilitiaNumbers() {
	return MaxNumberOfMilitia - TotalDiedMilitia;
}