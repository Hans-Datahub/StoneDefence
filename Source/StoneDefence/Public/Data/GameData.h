#pragma once

#include "CoreMinimal.h"
#include "GameData.generated.h"

USTRUCT()
struct FGameInstanceDatas {
	GENERATED_BODY()

	FGameInstanceDatas();

	void Init();

	bool IsValid();

	UPROPERTY(SaveGame)
	int32 RemainNumberOfMonster;//NumberOfMonster->RemainNumberOfMonster

	//当前关卡最大怪物数量
	UPROPERTY(SaveGame)
		int32 MaxNumberOfMonster;

	UPROPERTY(SaveGame)
		int32 GameDifficulty;

	UPROPERTY(SaveGame)
		int32 MaxStage;

	UPROPERTY(Transient)
		uint8 bAllMainTowersDied : 1;

	UPROPERTY(Transient)
		uint8 bGameOver : 1;

	UPROPERTY(Transient)
		uint8 bCurrentLevelMissionSuccess : 1;

	UPROPERTY(Transient)
		uint8 bTimeFreezed : 1;

	//怪物生成间隔
	UPROPERTY(SaveGame)
		float SpawnTimeInterval; //TimeInterval -> SpawnTimeInterval

	//生成当前一波怪物后过了多久
	UPROPERTY(SaveGame)
		float CurrentSpawnMonsterTime;

	UPROPERTY(SaveGame)
		int32 CurrentLevel;

	UPROPERTY(SaveGame)
		TArray<int32> MobNumberinCurrentStage;//记录有多少波和每波多少怪物
	//总倒计时
	UPROPERTY(SaveGame)
		float GameTimeCount;

	//最大时间
	UPROPERTY(SaveGame)
		float MaxGameTimeCount;

	UPROPERTY(SaveGame)
		float GlodGrowthTime;

	UPROPERTY(SaveGame)
		float MaxGlodGrowthTime;

	UPROPERTY(SaveGame)
		float KilledMobNumber;

	UPROPERTY(SaveGame)
		float KilledBossNumber;

	UPROPERTY(SaveGame)
		float TotalDiedTower;

	UPROPERTY(SaveGame)
		float TotalDiedMainTower;


	int32 GetRemainMobNumbers();

	float GetPercentageOfRemainMob();
	float GetMaxMobNumber();
	void ResetCurrentSpawn();
	FORCEINLINE bool bAllowSpawnMonster() { return CurrentSpawnMonsterTime >= SpawnTimeInterval; };

	void AssignedMonsterAmount();

	void StageDecision();

};