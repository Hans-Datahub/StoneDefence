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
	MaxNumberOfMilitia = 20;
	MaxNumberOfMarine = 1;
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
				//随机分配数量，并为剩余数量去掉本次分配的部分
				CurrentStageAssignedMilitiaNum = FMath::RandRange(EverageMilitiaNumInEachStage / 6, EverageMilitiaNumInEachStage);
				CurrentMilitiaNumber -= CurrentStageAssignedMilitiaNum;
			}
			else//若为最后一波，分配剩余所有单位
				CurrentStageAssignedMilitiaNum = CurrentMilitiaNumber;
			//将确定的数量加入数组
			MilitiaNumberinCurrentStage.Add(CurrentStageAssignedMilitiaNum);
		}
	}
	else
		MilitiaNumberinCurrentStage.Add(CurrentStageAssignedMilitiaNum);

	MilitiaCurrentStage = MilitiaNumberinCurrentStage.Num() - 1;
}

void FGameInstanceDatas::AssignedMarineAmount() {
	//int32 CurrentMarineNumber = MaxNumberOfMarine;
	//int32 CurrentStageNumber = MaxStage;
	//int32 CurrentStageAssignedMarineNum = 0;
	//if (CurrentMarineNumber > 1) {
	//	for (int32 i = 0; i < CurrentStageNumber; i++) {
	//		float EverageMarineNumInEachStage = (float)MaxNumberOfMarine / (float)CurrentStageNumber;
	//		CurrentStageNumber--;
	//		if (CurrentStageNumber > 1) {//若不是最后一波
	//			//随机分配数量，并为剩余数量去掉本次分配的部分
	//			CurrentStageAssignedMarineNum = FMath::RandRange(EverageMarineNumInEachStage / 6, EverageMarineNumInEachStage);
	//			CurrentMarineNumber -= CurrentStageAssignedMarineNum;
	//		}
	//		else//若为最后一波，分配剩余所有单位
	//			CurrentStageAssignedMarineNum = CurrentMarineNumber;
	//		//将确定的数量加入数组
	//		MilitiaNumberinCurrentStage.Add(CurrentStageAssignedMarineNum);
	//	}
	//}
	//else
	//	MarineNumberinCurrentStage.Add(CurrentStageAssignedMarineNum);
	
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