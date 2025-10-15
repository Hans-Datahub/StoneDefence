#include "Core/SelectLevelCore/TD_SelectLevelPlayerController.h"
#include "UI/SelectLevelUI/UMG/UI_SelectLevelMain.h"
#include "Core/GameCore/TD_GameCamera.h"
#include "Kismet/GameplayStatics.h"

void ATD_SelectLevelPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 绑定输入
    if (InputComponent)
    {
        InputComponent->BindAction("NavigateLeft", IE_Pressed, this, &ATD_SelectLevelPlayerController::NavigateLeft);
        InputComponent->BindAction("NavigateRight", IE_Pressed, this, &ATD_SelectLevelPlayerController::NavigateRight);
        InputComponent->BindAction("ConfirmSelection", IE_Pressed, this, &ATD_SelectLevelPlayerController::ConfirmSelection);
    }
}


void ATD_SelectLevelPlayerController::BeginPlay() {

    Super::BeginPlay();

    // 设置为UI-only模式：按键只影响UI，不影响Pawn
    //FInputModeUIOnly InputMode;
    //InputMode.SetWidgetToFocus(UUI_SelectLevelMain); // 聚焦到你的UI(UI暂时无内容，后续添加按键提示)
    //InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    //SetInputMode(InputMode);

    // 显示鼠标光标
    bShowMouseCursor = true;

    //初始化所有相机
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATD_GameCamera::StaticClass(), FoundCameras);
}

ATD_GameCamera* ATD_SelectLevelPlayerController::GetTargetCameraByIndex() {
    FString CameraName = FString::Printf(TEXT("Level%dCamera"), CurrentSelectedLevelIndex);
    for (AActor* TempCamera : FoundCameras) {
        if (TempCamera->GetActorLabel() == CameraName) {
            return Cast<ATD_GameCamera>(TempCamera);
        }
    }
    return nullptr;
}

void ATD_SelectLevelPlayerController::CameraMove() {
    ATD_GameCamera* TargetCamera = GetTargetCameraByIndex();
    this->SetViewTargetWithBlend(TargetCamera, 0.5f, EViewTargetBlendFunction::VTBlend_Linear);
    //镜头移动完成后置否
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { isCameraMoving = false; }, 0.6f, false);
}

void ATD_SelectLevelPlayerController::NavigateLeft() {
    if (isCameraMoving)
        return;
    isCameraMoving = true;
    CurrentSelectedLevelIndex--;
    if (CurrentSelectedLevelIndex < 1)
        CurrentSelectedLevelIndex = 4;
    CameraMove();
}

void ATD_SelectLevelPlayerController::NavigateRight() {
    if (isCameraMoving)
        return;
    isCameraMoving = true;
    CurrentSelectedLevelIndex++;
    if (CurrentSelectedLevelIndex > 4)
        CurrentSelectedLevelIndex = 1;
    CameraMove();    
} 

void ATD_SelectLevelPlayerController::ConfirmSelection() {
    UGameplayStatics::OpenLevel(GetWorld(), "Level1");
}