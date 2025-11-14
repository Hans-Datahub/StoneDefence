// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TD_GameMode.h"
#include "Character/CharacterCore/Militia.h"
#include "Character/CharacterCore/Marine.h"
#include "Selection/SelectionManager.h"
#include "LowPolyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ALowPolyGameMode : public ATD_GameMode
{
	GENERATED_BODY()

	ALowPolyGameMode();

public:

	UPROPERTY(BlueprintReadOnly, Category = "Selection")
	class ASelectionManager* SelectionManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection")
	TSubclassOf<ASelectionManager> SelectionManagerClass;

	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AMilitia* SpawnMilitia(int32 CharacterID, const FVector& Location, const FRotator& Rotator, int32 CharacterLevel = 1/*, const FGuid& Guid*/);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AMarine* SpawnMarine(int32 CharacterID, const FVector& Location, const FRotator& Rotator, int32 CharacterLevel = 1/*, const FGuid& Guid*/);
	
	void UpdateUnitSpawnRule(float DeltaSeconds);

	void InitStandardData() override;


};
