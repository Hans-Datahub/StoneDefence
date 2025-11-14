// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Engine/DataTable.h"
#include "TD_GameMode.generated.h"


class ATowers;
class AMonsters;
class ARuleOfCharacter;
class UDataTable;
class ATD_PlayerController;

UCLASS()
class STONEDEFENCE_API ATD_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ATD_GameMode();
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaSeconds);

	void InitDataFormArchives();
	virtual void InitStandardData(); //分配每波怪物数量

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		ATowers* SpawnTower(int32 CharacterID, const FVector& Location, const FRotator& Rotator, int32 CharacterLevel = 1/*, const FGuid& InCharacterGuid*/);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		AMonsters* SpawnMonster(int32 CharacterID, const FVector& Location, const FRotator& Rotator, int32 CharacterLevel = 1/*, const FGuid& InCharacterGuid*/);
	
	
protected:
	ARuleOfCharacter* SpawnCharacter(int32 CharacterID, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator, int32 CharacterLevel = 1/*, const FGuid& InCharacterGuid*/);
	
	template<class T>
	T* SpawnCharacter(int32 CharacterID, UDataTable* InCharacterData, const FVector& Loction, const FRotator& Rotator, int32 CharacterLevel = 1/*, const FGuid& InCharacterGuid = FGuid()*/)
	{
		return Cast<T>(SpawnCharacter(CharacterID, InCharacterData, Loction, Rotator, CharacterLevel/*, InCharacterGuid*/));
	}

	void SpawnTowerRule();
	void SpawnMainTowerRule();

	void UpdateMonsterSpawnRule(float DeltaSeconds);

	void UpdatePlayerData(float DeltaSeconds);

	void UpdateGameData(float DeltaSeconds);

	void UpdatePlayerSkill(float DeltaSeconds);
	//技能系统更新
	void UpdateSkill(float DeltaSeconds);

	void UpdateInventory(float DeltaSeconds);

	int32 GetTowerDifficultyParam_Level(UWorld* InWorld);
};
