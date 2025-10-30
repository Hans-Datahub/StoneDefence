// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Synty_Camera.generated.h"

UCLASS()
class STONEDEFENCE_API ASynty_Camera : public APawn
{
	GENERATED_BODY()

public:
	ASynty_Camera();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    /*--------------------------------------------------------------------------*/


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
        USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
        UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float MoveSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float ZoomSpeed = 500.0f;


    // 移动边界
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMovementBound")
        FVector2D HorizontalXBounds = FVector2D(-7000, -2000);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMovementBound")
        FVector2D HorizontalYBounds = FVector2D(-9000, -6500);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMovementBound")
        FVector2D VerticalBounds = FVector2D(1200, 2000);


    // 视角旋转限制
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float MouseSensitivity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float MinPitch = -70.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float MaxPitch = -20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float AccelerateRatio = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicParamaters")
        float SpeedTransitionTime = 0.2f;

    //用于平滑过渡状态中的动态速度倍率存储
    float CurrentSpeedMultiplier = 1.0f;


    // 输入变量
    FVector MovementInput;
    FVector2D MouseInput;
    FVector VerticalMovement = FVector::ZeroVector;
    FVector NewLocation = FVector::ZeroVector;
    bool bIsRightMousePressed = false;
    bool bIsAccelerating = false;

public:
    // 输入函数
    void MoveForward(float Value);
    void MoveRight(float Value);
    void MoveUp(float Value);

    // 摄像机控制接口
    void OnCameraControlStarted();
    void OnCameraControlEnded();

    // 鼠标输入处理（由PlayerController调用）
    void HandleMouseX(float Value);
    void HandleMouseY(float Value);

    // 键盘输入处理
    void OnShiftPressed();
    void OnShiftReleased();

    void UpdateCameraMovement(float DeltaTime);
    void UpdateCameraRotation(float DeltaTime);



    // 调试显示
    void DrawDebugBounds();

};
