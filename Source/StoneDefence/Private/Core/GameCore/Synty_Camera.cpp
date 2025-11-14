// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/Synty_Camera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/GameCore/TD_PlayerController.h"


// Sets default values
ASynty_Camera::ASynty_Camera()
{
    PrimaryActorTick.bCanEverTick = true;

    // 创建根组件
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // 创建弹簧臂
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 100.0f;
    SpringArm->SetRelativeRotation(FRotator(-40.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = false;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 3.0f;

    // 创建摄像机
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ASynty_Camera::BeginPlay()
{
	Super::BeginPlay();

    // 设置初始鼠标光标
    if (ATD_PlayerController* PC = Cast<ATD_PlayerController>(GetController()))
    {
        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;
    }
	
}

void ASynty_Camera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("CameraMoveForward", this, &ASynty_Camera::MoveForward);
    PlayerInputComponent->BindAxis("CameraMoveRight", this, &ASynty_Camera::MoveRight);
    PlayerInputComponent->BindAxis("CameraMoveUp", this, &ASynty_Camera::MoveUp);
}

void ASynty_Camera::MoveForward(float Value) { MovementInput.X = Value; }
void ASynty_Camera::MoveRight(float Value) { MovementInput.Y = Value; }
void ASynty_Camera::MoveUp(float Value) { MovementInput.Z = Value; }

// 在Tick中更新移动
void ASynty_Camera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 平滑速度过渡
    float TargetMultiplier = bIsAccelerating ? AccelerateRatio : 1.0f;
    CurrentSpeedMultiplier = FMath::FInterpTo(CurrentSpeedMultiplier, TargetMultiplier, DeltaTime, 1.0f / SpeedTransitionTime);

    UpdateCameraMovement(DeltaTime);
    UpdateCameraRotation(DeltaTime);

    // 绘制调试边界
    //DrawDebugBounds();
}

void ASynty_Camera::UpdateCameraMovement(float DeltaTime)
{
    if (!MovementInput.IsZero())
    {
        // 根据加速状态选择速度
        float CurrentMoveSpeed = MoveSpeed * CurrentSpeedMultiplier;
        float CurrentZoomSpeed = ZoomSpeed * CurrentSpeedMultiplier;

        // 基于摄像机方向计算移动方向
        FVector Forward = Camera->GetForwardVector();
        FVector Right = Camera->GetRightVector();

        // 忽略垂直分量
        Forward.Z = 0.0f;
        Right.Z = 0.0f;
        Forward.Normalize();//换为单位向量
        Right.Normalize();

        // 计算移动向量
        FVector Movement = (Forward * MovementInput.X + Right * MovementInput.Y).GetSafeNormal();//单位方向向量
        Movement *= CurrentMoveSpeed * DeltaTime;

        // 垂直移动
        VerticalMovement = FVector(0, 0, MovementInput.Z * CurrentZoomSpeed * DeltaTime);

        // 应用移动
        NewLocation = GetActorLocation() + Movement + VerticalMovement;

        // 应用边界限制
        NewLocation.X = FMath::Clamp(NewLocation.X, HorizontalXBounds.X, HorizontalXBounds.Y);
        NewLocation.Y = FMath::Clamp(NewLocation.Y, HorizontalYBounds.X, HorizontalYBounds.Y);
        NewLocation.Z = FMath::Clamp(NewLocation.Z, VerticalBounds.X, VerticalBounds.Y);


        SetActorLocation(NewLocation);
    }
}

void ASynty_Camera::UpdateCameraRotation(float DeltaTime)
{
    if (bIsRightMousePressed && !MouseInput.IsZero())
    {
        // 水平旋转（Yaw）- 旋转整个Pawn
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += MouseInput.X * MouseSensitivity;
        SetActorRotation(NewRotation);

        // 垂直旋转（Pitch）- 只旋转SpringArm，并限制角度
        FRotator SpringArmRotation = SpringArm->GetRelativeRotation();
        SpringArmRotation.Pitch = FMath::Clamp(
            SpringArmRotation.Pitch + (MouseInput.Y * MouseSensitivity),
            MinPitch,
            MaxPitch
        );
        SpringArm->SetRelativeRotation(SpringArmRotation);

        // 重置鼠标输入，准备接收下一帧的增量
        MouseInput = FVector2D::ZeroVector;
    }
}

void ASynty_Camera::OnCameraControlStarted()
{
    bIsRightMousePressed = true;
}

void ASynty_Camera::OnCameraControlEnded()
{
    bIsRightMousePressed = false;
    MouseInput = FVector2D::ZeroVector;
}

void ASynty_Camera::HandleMouseX(float Value)
{
    if (bIsRightMousePressed)
        MouseInput.X = Value;
}

void ASynty_Camera::HandleMouseY(float Value)
{
    if (bIsRightMousePressed)
        MouseInput.Y = Value;
}

void ASynty_Camera::OnShiftPressed()
{
    bIsAccelerating = true;
}

void ASynty_Camera::OnShiftReleased()
{
    bIsAccelerating = false;
}

void ASynty_Camera::DrawDebugBounds()
{
    if (GetWorld())
    {
        FVector Center = FVector(
            (HorizontalXBounds.X + HorizontalXBounds.Y) / 2.0f,
            (HorizontalYBounds.X + HorizontalYBounds.Y) / 2.0f,
            (VerticalBounds.X + VerticalBounds.Y) / 2.0f
        );

        FVector Extent = FVector(
            (HorizontalXBounds.Y - HorizontalXBounds.X) / 2.0f,
            (HorizontalYBounds.Y - HorizontalYBounds.X) / 2.0f,
            (VerticalBounds.Y - VerticalBounds.X) / 2.0f
        );

        FBox BoundsBox = FBox(Center - Extent, Center + Extent);

        DrawDebugBox(
            GetWorld(),
            Center,
            Extent,
            FColor::Green,
            false, // 不持久化
            -1.0f, // 只在一帧中显示
            0,     // 深度优先级
            20.0f   // 线宽
        );
    }
}
