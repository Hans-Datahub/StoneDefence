// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterCore/Monsters.h"
#include "Character/SteeringBehavior/SteeringBehaviorComponent.h"
#include "Units.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AUnits : public AMonsters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
	USteeringBehaviorComponent* SteeringComponent;


	AUnits();

	virtual void Tick(float DeltaTime)override;
	
};
