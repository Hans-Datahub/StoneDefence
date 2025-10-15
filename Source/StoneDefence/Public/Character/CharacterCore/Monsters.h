// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Monsters.generated.h"



UCLASS()
class STONEDEFENCE_API AMonsters : public ARuleOfCharacter
{
	GENERATED_BODY()

public:
	bool isFreezed;//用于速度状态判断

	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
	virtual EGameCharacterType::Type GetCharacterType();
	virtual ETeam GetTeamType();
	virtual void RegisterTeam();

	UFUNCTION()
	virtual void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);


};
