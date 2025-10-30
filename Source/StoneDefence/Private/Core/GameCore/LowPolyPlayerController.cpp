// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/LowPolyPlayerController.h"
#include "Selection/SelectionManager.h"
#include "Core/GameCore/LowPolyGameMode.h"

void ALowPolyPlayerController::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    ScreenEdgeMoveListen();
    UE_LOG(LogTemp, Warning, TEXT("当前控制模式：%d"), CurrentControlMode);
}

void ALowPolyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    //鼠标按键输入
    InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ALowPolyPlayerController::OnLeftMousePressed);
    InputComponent->BindAction("LeftMouse", IE_Released, this, &ALowPolyPlayerController::OnLeftMouseReleased);
    InputComponent->BindAction("RightMouse", IE_Pressed, this, &ALowPolyPlayerController::OnRightMousePressed);
    InputComponent->BindAction("RightMouse", IE_Released, this, &ALowPolyPlayerController::OnRightMouseReleased);

    // 鼠标移动输入
    InputComponent->BindAxis("MouseX", this, &ALowPolyPlayerController::OnMouseX);
    InputComponent->BindAxis("MouseY", this, &ALowPolyPlayerController::OnMouseY);


    //模式切换
    InputComponent->BindAction("Shift", IE_Pressed, this, &ALowPolyPlayerController::OnShiftPressed);
    InputComponent->BindAction("Shift", IE_Released, this, &ALowPolyPlayerController::OnShiftReleased);
    InputComponent->BindAction("Ctrl", IE_Pressed, this, &ALowPolyPlayerController::OnCtrlPressed);
    InputComponent->BindAction("Ctrl", IE_Released, this, &ALowPolyPlayerController::OnCtrlReleased);
}

void ALowPolyPlayerController::OnLeftMousePressed()
{
    // 如果正在摄像机控制模式，忽略左键选择
    if (CurrentControlMode == EControlMode::Camera) return;

    FVector2D MousePosition;
    if (GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        if (ASelectionManager* Manager = GetSelectionManager())
        {
            Manager->StartSelection(MousePosition);
        }
    }
   bShowMouseCursor = true; // 确保鼠标光标可见
}

void ALowPolyPlayerController::OnLeftMouseReleased()
{
    if (CurrentControlMode == EControlMode::Camera) return;

    if (ASelectionManager* Manager = GetSelectionManager())
    {
        Manager->FinishSelection();
    }
}

void ALowPolyPlayerController::OnRightMousePressed()
{
    bIsRightMousePressed = true;
    CurrentControlMode = EControlMode::Camera;
    bCameraHasInputPriority = true;

    // 切换到摄像机控制模式
    bShowMouseCursor = false;
    bEnableClickEvents = false;

    // 设置输入模式，锁定鼠标
    SetInputMode(FInputModeGameOnly());

    // 通知摄像机开始控制
    if (ASynty_Camera* Camera = GetSyntyCamera())
    {
        Camera->OnCameraControlStarted();
    }
}

void ALowPolyPlayerController::OnRightMouseReleased()
{
    bIsRightMousePressed = false;
    CurrentControlMode = EControlMode::Selection;
    bCameraHasInputPriority = false;

    // 切换回选择模式
    bShowMouseCursor = true;
    bEnableClickEvents = true;

    // 设置输入模式，解锁鼠标
    FInputModeGameOnly InputMode;
    InputMode.SetConsumeCaptureMouseDown(false);
    SetInputMode(InputMode);


    // 通知摄像机结束控制
    if (ASynty_Camera* Camera = GetSyntyCamera())
    {
        Camera->OnCameraControlEnded();
    }
}

void ALowPolyPlayerController::OnMouseX(float Value)
{
    // 优先处理摄像机控制
    if (CurrentControlMode == EControlMode::Camera || bIsRightMousePressed)
    {
        if (ASynty_Camera* Camera = GetSyntyCamera())
        {
            Camera->HandleMouseX(Value);
        }
    }
    else if (ASelectionManager* Manager = GetSelectionManager())
    {
        // 更新框选
        FVector2D MousePosition;
        double X, Y;
        if (GetMousePosition(X,Y))
        {
            MousePosition.X = X;
            MousePosition.Y = Y;
            Manager->UpdateSelection(MousePosition);
        }
    }
}

void ALowPolyPlayerController::OnMouseY(float Value)
{
    // 优先处理摄像机控制
    if (CurrentControlMode == EControlMode::Camera || bIsRightMousePressed)
    {
        if (ASynty_Camera* Camera = GetSyntyCamera())
        {
            Camera->HandleMouseY(Value);
        }
    }
}

void ALowPolyPlayerController::OnShiftPressed()
{
    bIsShiftPressed = true;
    // 输入优先级：摄像机控制优先
    if (CurrentControlMode == EControlMode::Camera || bCameraHasInputPriority) {
        // 优先给摄像机加速
        if (ASynty_Camera* Camera = GetSyntyCamera())
            Camera->OnShiftPressed();
    }
    else {
        // 其次处理选择模式
        if (ASelectionManager* Manager = GetSelectionManager())
            Manager->SetSelectionMode(ELowpolySelectionMode::Additive);
    }
}

void ALowPolyPlayerController::OnShiftReleased()
{
    bIsShiftPressed = false;
    // 输入优先级：摄像机控制优先
    if (CurrentControlMode == EControlMode::Camera || bCameraHasInputPriority)
        if (ASynty_Camera* Camera = GetSyntyCamera())
            Camera->OnShiftReleased();

    // 总是恢复选择模式到正常
    if (ASelectionManager* Manager = GetSelectionManager())
        Manager->SetSelectionMode(ELowpolySelectionMode::Normal);
}


void ALowPolyPlayerController::OnCtrlPressed()
{
    if (ASelectionManager* Manager = GetSelectionManager())
    {
        Manager->SetSelectionMode(ELowpolySelectionMode::Subtractive);
    }
}

void ALowPolyPlayerController::OnCtrlReleased()
{
    if (ASelectionManager* Manager = GetSelectionManager())
    {
        Manager->SetSelectionMode(ELowpolySelectionMode::Normal);
    }
}




ASelectionManager* ALowPolyPlayerController::GetSelectionManager() const
{
    if (ALowPolyGameMode* GameMode = Cast<ALowPolyGameMode>(GetWorld()->GetAuthGameMode()))
    {
        return GameMode->SelectionManager;
    }
    return nullptr;
}

ASynty_Camera* ALowPolyPlayerController::GetSyntyCamera() const
{
    return Cast<ASynty_Camera>(GetPawn());
}

void ALowPolyPlayerController::ScreenEdgeMoveListen() {
    //若没有右键旋转摄像头，开始边缘检测屏幕移动
    if (CurrentControlMode == EControlMode::Selection) {
        float TempMoveSpeed = EdgeScreenMoveSpeed;
        if (ASynty_Camera* Camera = GetSyntyCamera()) {
            if (Camera->bIsAccelerating)
                TempMoveSpeed = EdgeScreenMoveSpeed * MoveSpeedMultiplier;
        }   
        //通知边缘移动插件
        ScreenMoveUnits.ListenScreenMove(this, TempMoveSpeed);
    }
}