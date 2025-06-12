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

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		ATowers* SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		AMonsters* SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);


	void SpawnRuleOfMonster(float DeltaSeconds);
	void SpawnTowerRule();
	void SpawnMainTowerRule();
	
protected:
	ARuleOfCharacter* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator);
	template<class T>
	T* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator) {
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

	//技能系统更新
	void UpdateSkill(float DeltaSeconds);

private:
	int32 GetTowerDifficultyParam_Level(UWorld* InWorld);
};
