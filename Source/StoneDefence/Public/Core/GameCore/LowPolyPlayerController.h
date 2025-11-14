// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Core/GameCore/Synty_Camera.h"
#include "../StoneDefenceType.h"
#include "LowPolyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ALowPolyPlayerController : public ATD_PlayerController
{
	GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;

    virtual void Tick(float DeltaSeconds)override;

private:
    void OnLeftMousePressed();
    void OnLeftMouseReleased();
    void OnRightMousePressed();
    void OnRightMouseReleased();

    void OnMouseX(float Value);
    void OnMouseY(float Value);

    void OnShiftPressed();
    void OnShiftReleased();

    void OnCtrlPressed();
    void OnCtrlReleased();

    class ASelectionManager* GetSelectionManager() const;
    ASynty_Camera* GetSyntyCamera() const;
    void ScreenEdgeMoveListen();

private:
    // 当前控制模式
    EControlMode CurrentControlMode = EControlMode::Selection;

    // 鼠标状态
    bool bIsRightMousePressed = false;
    bool bIsShiftPressed = false;

    // 输入优先级：摄像机控制优先于选择
    bool bCameraHasInputPriority = false;

    // 记录右键按下时的初始屏幕位置（用于区分点击/拖动）
    FVector2D RightMouseDownScreenPos;
    // 判定为"点击"的最大鼠标移动阈值（像素，可根据需求调整）
    const float ClickMaxMovementThreshold = 3.0f;
    // 标记是否已触发拖动（避免拖动后误判为点击）
    bool bIsRightMouseDragged = false;


private:
    void CheckRightClickNavMeshPosition();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeScreenMove")
    float EdgeScreenMoveSpeed = 50.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeScreenMove")
        float MoveSpeedMultiplier = 2.0f;
	
};
