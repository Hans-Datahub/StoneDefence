// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SelectLevelCore/TD_SelectLevelGameMode.h"
#include "UI/SelectLevelUI/Core/TD_SelectLevelHUD.h"

#include "Core/GameCore/TD_GameCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SelectLevelCore/TD_SelectLevelPlayerController.h"

enum EViewTargetBlendFunction;


ATD_SelectLevelGameMode::ATD_SelectLevelGameMode() {
	HUDClass = ATD_SelectLevelHUD::StaticClass();
    PlayerControllerClass = ATD_SelectLevelPlayerController::StaticClass();
}

void ATD_SelectLevelGameMode::BeginPlay() {
    Super::BeginPlay();

    // 查找场景中的自定义摄像机
    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATD_GameCamera::StaticClass(), FoundCameras);

    if (GetWorld()->GetFirstPlayerController() && FoundCameras.Num() > 0) {
        ATD_SelectLevelPlayerController* PlayerController = Cast<ATD_SelectLevelPlayerController>(GetWorld()->GetFirstPlayerController());
        // 设置主摄像机为视图目标
        PlayerController->SetViewTarget(GetCamera(FoundCameras, "SelectCamera"));
        //平滑切换第一关摄像头 
        PlayerController->SetViewTargetWithBlend(GetCamera(FoundCameras, "Level1Camera"), 1.5f, EViewTargetBlendFunction::VTBlend_Linear);
    }
}

ATD_GameCamera* ATD_SelectLevelGameMode::GetCamera(TArray<AActor*> FoundCameras, FString CameraName) {
    for (AActor* TempCamera : FoundCameras) {
        if (TempCamera->GetActorLabel() == CameraName)
            return Cast<ATD_GameCamera>(TempCamera);
    }
    return nullptr;
} 

/*
void PositionPrediction() {
    FVector2D EnemyPosition;
    FVector2D EnemyMoveDirection;
    FVector2D PlayerPosition;
    FVector2D FinalGrenadeDropPosition;
    FVector2D TempDropPosition;
    FVector2D EnemyPredictionPosition;
    FVector2D EnemyPlayerDistance;
    float PredictPointDropPointDist;
    float GrenadeFlyTime;
    float EnemySpeed;
    float GrenadeFlySpeed;
    float GrenadeExpoRange;

    //计算第一次递归的需要的基础数据
    TempDropPosition = EnemyPosition;
    EnemyPlayerDistance = TempDropPosition - PlayerPosition;
    GrenadeFlyTime = EnemyPlayerDistance.Size() / GrenadeFlySpeed;
    EnemyPredictionPosition = EnemyPosition + EnemyMoveDirection * EnemySpeed * GrenadeFlyTime;

    //计算与预测位置的差值
    PredictPointDropPointDist = (EnemyPredictionPosition - TempDropPosition).Size();

    if (GrenadeExpoRange < PredictPointDropPointDist) {
        //移动临时预测落点到预测敌人位置上
        TempDropPosition = EnemyPredictionPosition;
        EnemyPlayerDistance = TempDropPosition - PlayerPosition;
        GrenadeFlyTime = EnemyPlayerDistance.Size() / GrenadeFlySpeed;
        EnemyPredictionPosition = EnemyPosition + EnemyMoveDirection * EnemySpeed * GrenadeFlyTime;
        //再次计算与预测位置的差值
        PredictPointDropPointDist = (EnemyPredictionPosition - TempDropPosition).Size();

        if (1) {
            
        }

    }
    return ;
}

FVector2D PositionPredictionRecursive(
    const FVector2D& EnemyPosition,
    const FVector2D& EnemyMoveDirection,
    const FVector2D& PlayerPosition,
    float EnemySpeed,
    float GrenadeFlySpeed,
    float GrenadeExpoRange,
    int MaxIterations = 10,  // 防止无限递归
    int CurrentIteration = 0
) {
    // 递归终止条件：达到最大迭代次数
    if (CurrentIteration >= MaxIterations) {
        return EnemyPosition + EnemyMoveDirection * EnemySpeed *
            (EnemyPosition - PlayerPosition).Size() / GrenadeFlySpeed;
    }

    // 计算当前迭代的预测
    FVector2D TempDropPosition = (CurrentIteration == 0) ? EnemyPosition :
        EnemyPosition + EnemyMoveDirection * EnemySpeed *
        (EnemyPosition - PlayerPosition).Size() / GrenadeFlySpeed;

    FVector2D EnemyPlayerDistance = TempDropPosition - PlayerPosition;
    float GrenadeFlyTime = EnemyPlayerDistance.Size() / GrenadeFlySpeed;
    FVector2D EnemyPredictionPosition = EnemyPosition + EnemyMoveDirection * EnemySpeed * GrenadeFlyTime;

    // 计算预测位置与临时落点的距离
    float PredictPointDropPointDist = (EnemyPredictionPosition - TempDropPosition).Size();

    // 如果爆炸范围不足以覆盖预测位置，继续递归
    if (GrenadeExpoRange < PredictPointDropPointDist) {
        return PositionPredictionRecursive(
            EnemyPosition,
            EnemyMoveDirection,
            PlayerPosition,
            EnemySpeed,
            GrenadeFlySpeed,
            GrenadeExpoRange,
            MaxIterations,
            CurrentIteration + 1
        );
    }

    // 满足条件，返回最终的预测位置
    return EnemyPredictionPosition;
}

// 包装函数，保持原有接口
void PositionPrediction() {
    FVector2D EnemyPosition;
    FVector2D EnemyMoveDirection;
    FVector2D PlayerPosition;
    float EnemySpeed;
    float GrenadeFlySpeed;
    float GrenadeExpoRange;

    // 使用递归计算最终落点
    FVector2D FinalGrenadeDropPosition = PositionPredictionRecursive(
        EnemyPosition,
        EnemyMoveDirection,
        PlayerPosition,
        EnemySpeed,
        GrenadeFlySpeed,
        GrenadeExpoRange
    );

    // 这里可以继续使用 FinalGrenadeDropPosition
    return;
}

*/
