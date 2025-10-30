// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameCore/TD_GameState.h"
#include "Character/CharacterCore/Militia.h"
#include "Character/CharacterCore/Marine.h"
#include "LowPolyGameState.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ALowPolyGameState : public ATD_GameState
{
	GENERATED_BODY()

	ALowPolyGameState();

public:
	UPROPERTY()
		UDataTable* AIMilitiaCharacterData;
	UPROPERTY()
		UDataTable* AIMarineCharacterData;
	

	//缓存池
	TArray<FCharacterData*> CacheMilitiaDatas;
	TArray<FCharacterData*> CacheMarineDatas;


	void BeginPlay();
	void AssignExistedUnit();
	const TArray<FCharacterData*>& ALowPolyGameState::GetMilitiaDataArrayFromTable();
	const TArray<FCharacterData*>& ALowPolyGameState::GetMarineDataArrayFromTable();
};
