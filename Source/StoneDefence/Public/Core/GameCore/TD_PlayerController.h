// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool/ScreenMoveUnits.h"
#include "GameFramework/PlayerController.h"
#include "Global/UI_Data.h"
#include "../TD_GameMode.h"
#include "TD_PlayerController.generated.h"




DECLARE_DELEGATE_OneParam(FSkillAddingDelegate, FGuid)
DECLARE_DELEGATE_TwoParams(FProjectileSpawnDelegate, FGuid, UClass*)

UCLASS()
class STONEDEFENCE_API ATD_PlayerController : public APlayerController
{
	GENERATED_BODY()
	ATD_PlayerController();

public:
	//按下与松开鼠标中键代理
	FSimpleDelegate EventMouseMiddlePressed;
	FSimpleDelegate EventMouseMiddleReleased;
	//技能添加代理
	FSkillAddingDelegate SkillAddingDelegate;
	FSkillAddingDelegate SkillRemovingDelegate;

	FProjectileSpawnDelegate ProjectileSpawnDelegate;
	UFUNCTION()
		ATowers* SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);
	UFUNCTION()
		AMonsters* SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator);

	class ATD_GameMode* GetGameMode();

	//Tick函数
	virtual void Tick(float DeltaSeconds)override;

	//初始化操作
	virtual void BeginPlay();

	//锁定鼠标
	void SetInputModeGameAndUI();

	//允许玩家更新自己的绑定相应
	virtual void SetupInputComponent()override;

	//鼠标滚轮控制声明
	void MouseWheelUp();	
	void MouseWheelDown();

	void MouseMiddleButtonPressed();
	void MouseMiddleButtonReleased();

	const FHitResult& GetHitResult();

	UFUNCTION()
	void AddSkillSlot_Server(const FGuid& CharacterID, const FGuid& SlotID);
	UFUNCTION()
	void RemoveSkillSlot_Server(const FGuid& CharacterID, const FGuid& SlotID);
	UFUNCTION()
	void Spawn_Projectile_Client(const FGuid& CharacterID, const int32 &SkillID);
	UFUNCTION()
	void UpdateInventory_Client(const FGuid& InventorySlotGUID, bool bInCD);
protected:
	FScreenMoveUnits ScreenMoveUnits;

	FHitResult MouseTraceHit;
};
