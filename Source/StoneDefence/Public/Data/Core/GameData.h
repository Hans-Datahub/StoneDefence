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

	//��ǰ�ؿ�����������
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

	//�������ɼ��
	UPROPERTY(SaveGame)
		float SpawnTimeInterval; //TimeInterval -> SpawnTimeInterval

	//���ɵ�ǰһ���������˶��
	UPROPERTY(SaveGame)
		float CurrentSpawnMonsterTime;

	UPROPERTY(SaveGame)
		int32 CurrentLevel;

	UPROPERTY(SaveGame)
		TArray<int32> MobNumberinCurrentStage;//��¼�ж��ٲ���ÿ�����ٹ���
	//�ܵ���ʱ
	UPROPERTY(SaveGame)
		float GameTimeCount;
	//���ʱ��
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