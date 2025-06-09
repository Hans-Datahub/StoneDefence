// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool/ScreenMoveUnits.h"
#include "GameFramework/PlayerController.h"
#include "Global/UI_Data.h"
#include "../TD_GameMode.h"
#include "TD_PlayerController.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FSkillAddingDelegate, FGuid)
DECLARE_DELEGATE_TwoParams(FProjectileSpawnDelegate, FGuid, UClass*)

UCLASS()
class STONEDEFENCE_API ATD_PlayerController : public APlayerController
{
	GENERATED_BODY()
	ATD_PlayerController();

public:
	//�������ɿ�����м�����
	FSimpleDelegate EventMouseMiddlePressed;
	FSimpleDelegate EventMouseMiddleReleased;
	//������Ӵ���
	FSkillAddingDelegate SkillAddingDelegate;

	FProjectileSpawnDelegate ProjectileSpawnDelegate;
	UFUNCTION()
		ATowers* SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION()
		AMonsters* SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);

	class ATD_GameMode* GetGameMode();

	//Tick����
	virtual void Tick(float DeltaSeconds)override;

	//��ʼ������
	virtual void BeginPlay();

	//�������
	void SetInputModeGameAndUI();

	//������Ҹ����Լ��İ���Ӧ
	virtual void SetupInputComponent()override;

	//�����ֿ�������
	void MouseWheelUp();	
	void MouseWheelDown();

	void MouseMiddleButtonPressed();
	void MouseMiddleButtonReleased();

	const FHitResult& GetHitResult();

	UFUNCTION()
	void AddSkillSlot_Client(const FGuid& SlotID);
	UFUNCTION()
	void Spawn_Projectile_Client(const FGuid& CharacterID, UClass* InClass);
protected:
	FScreenMoveUnits ScreenMoveUnits;

	FHitResult MouseTraceHit;
};
