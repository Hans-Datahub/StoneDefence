// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/LowPolyPlayerController.h"
#include "Selection/SelectionManager.h"
#include "Core/GameCore/LowPolyGameMode.h"
#include "Core/GameCore/LowPolyGameState.h"
#include "NavigationSystem.h"

void ALowPolyPlayerController::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    ScreenEdgeMoveListen();
    //UE_LOG(LogTemp, Warning, TEXT("当前控制模式：%d"), CurrentControlMode);
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


    // 记录右键按下时的屏幕位置（关键：用于后续判断是否为点击）
    GetMousePosition(RightMouseDownScreenPos.X, RightMouseDownScreenPos.Y);
    // 重置拖动标记
    bIsRightMouseDragged = false;



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



    // 核心判断：若未拖动（或移动距离极小），视为点击
    if (!bIsRightMouseDragged)
    {
        // 二次验证：计算按下到释放的移动距离（防止微小抖动误判）
        FVector2D RightMouseUpScreenPos;
        GetMousePosition(RightMouseUpScreenPos.X, RightMouseUpScreenPos.Y);
        float MoveDistance = FVector2D::Distance(RightMouseDownScreenPos, RightMouseUpScreenPos);

        if (MoveDistance <= ClickMaxMovementThreshold)
        {
            // 执行右键点击逻辑：获取位置并判断NavMesh
            CheckRightClickNavMeshPosition();
        }
    }



}

void ALowPolyPlayerController::OnMouseX(float Value)
{
    // 优先处理摄像机控制
    if (CurrentControlMode == EControlMode::Camera || bIsRightMousePressed)
    {
        // 若右键按下且鼠标有移动，标记为拖动
        if (FMath::Abs(Value) > 0)  //是否需要设置为非零来过滤抖动？       
            bIsRightMouseDragged = true;

        if (ASynty_Camera* Camera = GetSyntyCamera())
            Camera->HandleMouseX(Value);

    }
    else if (ASelectionManager* Manager = GetSelectionManager()){
        // 更新框选
        FVector2D MousePosition;
        double X, Y;

        if (GetMousePosition(X,Y)){
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
        if (FMath::Abs(Value) > 0)
            bIsRightMouseDragged = true;

        if (ASynty_Camera* Camera = GetSyntyCamera())
            Camera->HandleMouseY(Value);
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


void ALowPolyPlayerController::CheckRightClickNavMeshPosition()
{
    UE_LOG(LogTemp, Log, TEXT("Task:右键已点击"));

    // 获取右键点击的屏幕坐标
    FVector2D ClickScreenPos;
    if (!GetMousePosition(ClickScreenPos.X, ClickScreenPos.Y))
        return; // 获取失败则退出

    // 屏幕坐标转世界射线检测（获取点击的世界位置）
    FHitResult WorldHitResult;
    bool bHit = GetHitResultAtScreenPosition(
        ClickScreenPos,
        ECC_WorldStatic, // 检测碰撞通道（根据游戏需求调整）
        false,
        WorldHitResult
    );
    if (!bHit)
        return; // 未命中任何物体则退出

    FVector ClickWorldPos = WorldHitResult.Location; // 点击的世界位置

    // 判断该位置是否在NavMesh中
    bool bIsInNavMesh = false;
    if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld()))
    {
        // 检查位置是否在NavMesh上（允许微小误差）
        FNavLocation NavLocation;
        bIsInNavMesh = NavSystem->ProjectPointToNavigation(
            ClickWorldPos,
            NavLocation,
            FVector(50.0f, 50.0f, 50.0f) // 检测范围（根据需求调整）
        );
    }

    // 根据结果执行后续逻辑（示例：输出日志）
    if (bIsInNavMesh)
    {
        //UE_LOG(LogTemp, Log, TEXT("右键点击位置在NavMesh中：%s"), *ClickWorldPos.ToString());

        if (ALowPolyGameState* TempState = GetWorld()->GetGameState<ALowPolyGameState>()) {
            if (ASelectionManager* SelManager = GetSelectionManager())
            {
                SelManager->GenerateAndAssignScatteredTargets(ClickWorldPos, GetWorld(), 50.f);

                const TArray<USelectableComponent*>& SelectedComponents = SelManager->CurrentlySelectedUnits;
                for (USelectableComponent* SelComp : SelectedComponents)
                {
                    if (!SelComp) continue; // 跳过空组件

                    // 从组件获取所属的Actor（ARuleOfCharacter）
                    ARuleOfCharacter* ComponentOwner = Cast<ARuleOfCharacter>(SelComp->GetOwner());
                    if (!ComponentOwner)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("选中的组件不属于ARuleOfCharacter类型"));
                        continue;
                    }

                    // 确保GUID有效
                    if (!ComponentOwner->GUID.IsValid())
                    {
                        UE_LOG(LogTemp, Warning, TEXT("单位 %s 的GUID无效"), *ComponentOwner->GetName());
                        continue;
                    }

                    // 更新CharacterData中的移动目标
                    FCharacterData& CharacterData = TempState->GetCharacterData(ComponentOwner->GUID);
                    if (CharacterData.IsValid())
                    {
                        CharacterData.LocationToMove = ClickWorldPos;
                        UE_LOG(LogTemp, Log, TEXT("Task:点击位置已传输至CharacterData.LocationToMove"));
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("未找到GUID为 %s 的CharacterData,目标移动位置读写失败"), *ComponentOwner->GUID.ToString());
                    }
                }
            }
        }        
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("右键点击位置不在NavMesh中：%s"), *ClickWorldPos.ToString());
        // 执行NavMesh外的逻辑（如提示不可放置等）
    }
}