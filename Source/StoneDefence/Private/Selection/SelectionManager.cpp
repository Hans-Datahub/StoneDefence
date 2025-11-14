#include "Selection/SelectionManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Core/GameCore/LowPolyGameState.h"
#include "NavigationSystem.h"
#include "../../StoneDefenceUtils.h"
#include "Character/CharacterCore/Towers.h"

ASelectionManager::ASelectionManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASelectionManager::BeginPlay()
{
    Super::BeginPlay();
    CreateSelectionBoxWidget();
}

void ASelectionManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSelecting)
    {
        UpdateSelectionBoxVisual();
    }
}

void ASelectionManager::StartSelection(const FVector2D& ScreenPosition)
{
    bIsSelecting = true;
    SelectionStart = ScreenPosition;
    SelectionEnd = ScreenPosition;

    // 普通模式下先清除之前的选择
    if (CurrentSelectionMode == ELowpolySelectionMode::Normal)
    {
        ClearAllSelections();
    }
}

void ASelectionManager::UpdateSelection(const FVector2D& CurrentScreenPosition)
{
    if (!bIsSelecting) return;

    SelectionEnd = CurrentScreenPosition;
}

void ASelectionManager::FinishSelection()
{
    if (!bIsSelecting) return;

    bIsSelecting = false;

    // 获取选择框内的单位
    TArray<USelectableComponent*> SelectedUnits = GetUnitsInSelectionArea();

    // 应用选择
    ApplySelection(SelectedUnits);

    // 隐藏选择框
    if (SelectionBoxWidget)
    {
        SelectionBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

TArray<USelectableComponent*> ASelectionManager::GetUnitsInSelectionArea() const
{
    TArray<USelectableComponent*> Result;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return Result;

    // 获取视口大小
    int32 ViewportSizeX, ViewportSizeY;
    PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

    // 计算选择框的屏幕范围（标准化）
    FVector2D NormalizedStart = FVector2D(
        FMath::Min(SelectionStart.X, SelectionEnd.X) / ViewportSizeX,
        FMath::Min(SelectionStart.Y, SelectionEnd.Y) / ViewportSizeY
    );

    FVector2D NormalizedEnd = FVector2D(
        FMath::Max(SelectionStart.X, SelectionEnd.X) / ViewportSizeX,
        FMath::Max(SelectionStart.Y, SelectionEnd.Y) / ViewportSizeY
    );

    // 获取所有可选中角色单位
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

    for (AActor* Actor : AllActors)
    {
        //若无法选中，跳过处理
        USelectableComponent* Selectable = Actor->FindComponentByClass<USelectableComponent>();
        if (!Selectable) { continue; }
        //若单位死亡，跳过处理
        if (ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(Actor))
            if (Character->IsDeath()) { continue; }


        // 将单位位置转换为屏幕坐标
        FVector2D ScreenPosition;
        PC->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenPosition);

        // 标准化屏幕坐标
        FVector2D NormalizedScreenPos = FVector2D(
            ScreenPosition.X / ViewportSizeX,
            ScreenPosition.Y / ViewportSizeY
        );

        // 检查是否在选择框内
        if (NormalizedScreenPos.X >= NormalizedStart.X &&
            NormalizedScreenPos.X <= NormalizedEnd.X &&
            NormalizedScreenPos.Y >= NormalizedStart.Y &&
            NormalizedScreenPos.Y <= NormalizedEnd.Y)
        {
            Result.Add(Selectable);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("选中单位数量: %d"), Result.Num());
    return Result;
}

void ASelectionManager::ApplySelection(const TArray<USelectableComponent*>& NewSelection)
{
    switch (CurrentSelectionMode)
    {
    case ELowpolySelectionMode::Normal:
        // 已经清除了，直接选择新单位
        for (USelectableComponent* Unit : NewSelection)
        {
            Unit->SetSelected(true);
            ToggleSelectionMark(Unit, true);
            CurrentlySelectedUnits.Add(Unit);
        }
        break;

    case ELowpolySelectionMode::Additive:
        // 追加选择
        for (USelectableComponent* Unit : NewSelection)
        {
            if (!Unit->bIsSelected)
            {
                Unit->SetSelected(true);
                ToggleSelectionMark(Unit, true);
                CurrentlySelectedUnits.Add(Unit);
            }
        }
        break;

    case ELowpolySelectionMode::Subtractive:
        // 取消选择
        for (USelectableComponent* Unit : NewSelection)
        {
            if (Unit->bIsSelected)
            {
                Unit->SetSelected(false);
                ToggleSelectionMark(Unit, false);
                CurrentlySelectedUnits.Remove(Unit);
            }
        }
        break;
    }
}

void ASelectionManager::ToggleSelectionMark(USelectableComponent* Unit, bool bVisible) {
    TArray<UActorComponent*> SelecitonMark = Unit->GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("SelectionMark"));
    for (UActorComponent* MarkComponent : SelecitonMark) {//实际上就一个Component
        if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(MarkComponent))
            PrimitiveComp->SetVisibility(bVisible);
    }
}

void ASelectionManager::ClearAllSelections()
{
    for (USelectableComponent* Unit : CurrentlySelectedUnits)
    {
        Unit->SetSelected(false);
        ToggleSelectionMark(Unit, false);
    }
    CurrentlySelectedUnits.Empty();
}

void ASelectionManager::CreateSelectionBoxWidget()
{
    if (SelectionBoxWidget) return;

    // 创建简单的选择框UI
    FStringClassReference WidgetClassRef(TEXT("/Game/UI/Game/Selection/Selection_BP.Selection_BP_C"));
    if (UClass* WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>())
    {
        SelectionBoxWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
        if (SelectionBoxWidget)
        {
            SelectionBoxWidget->AddToViewport();
            SelectionBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
    
void ASelectionManager::UpdateSelectionBoxVisual()
{
    if (!SelectionBoxWidget) return;
    SelectionBoxWidget->SetVisibility(ESlateVisibility::Visible);

    // 更新选择框位置和大小
    if (UImage* SelectionImage = Cast<UImage>(SelectionBoxWidget->GetWidgetFromName(TEXT("SelectionImage")))) {
        if (UCanvasPanelSlot* BorderSlot = Cast<UCanvasPanelSlot>(SelectionImage->Slot))
        {
            //获取ViewPort和Canvas的实际大小
            FVector2D ViewPortSize = FVector2D::ZeroVector;
            GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->ViewportClient->GetViewportSize(ViewPortSize);
            UCanvasPanel* Canvas = Cast<UCanvasPanel>(SelectionImage->GetParent());
            FVector2D CanvasSize = ViewPortSize; // 默认使用视口大小
            if (Canvas && Canvas->GetCachedGeometry().GetLocalSize() != FVector2D::ZeroVector)
                CanvasSize = Canvas->GetCachedGeometry().GetLocalSize();


            double TunedStartX, TunedStartY, TunedEndX, TunedEndY;
            TunedStartX = CanvasSize.X * (SelectionStart.X / ViewPortSize.X);
            TunedStartY = CanvasSize.Y * (SelectionStart.Y / ViewPortSize.Y);
            TunedEndX = CanvasSize.X * (SelectionEnd.X / ViewPortSize.X);
            TunedEndY = CanvasSize.Y * (SelectionEnd.Y / ViewPortSize.Y);


            TunedStart = FVector2D(TunedStartX, TunedStartY);
            TunedEnd = FVector2D(TunedEndX, TunedEndY);

            FVector2D Position = FVector2D(
                (FMath::Min(TunedStartX, TunedEndX)),
                (FMath::Min(TunedStartY, TunedEndY))
            );

            FVector2D Size = FVector2D(
                (FMath::Abs(TunedEndX - TunedStartX)),
                (FMath::Abs(TunedEndY - TunedStartY))
            );

            BorderSlot->SetPosition(Position);
            BorderSlot->SetSize(Size);
        }
    }
}


/*--------------------------------------------------------------*/
/*-------------------------移动目标点计算-------------------------*/
/*-------------------------------------------------------------*/

void ASelectionManager::GenerateAndAssignScatteredTargets(FVector CenterPos, UWorld* World, float UnitRadius)
{ 
    if (CurrentlySelectedUnits.Num() == 0 || !World) return;

    // 1. 计算关键参数
    const float MinSafeDistance = 2 * UnitRadius; // 单位间最小安全距离（避免碰撞箱重叠）
    const int32 UnitCount = CurrentlySelectedUnits.Num();

    // 2. 生成候选点（围绕中心的多层圆形分布）
    TArray<FVector> Candidates = GenerateCandidatePoints(CenterPos, UnitCount, MinSafeDistance);

    // 3. 验证候选点是否在NavMesh上（过滤静态障碍）
    TArray<FVector> ValidPoints = ValidatePointsOnNavMesh(Candidates, World);

    // 4. 若有效点不足，扩大范围重试（最多3次）
    int32 RetryCount = 0;
    while (ValidPoints.Num() < UnitCount && RetryCount < 3)
    {
        RetryCount++;
        Candidates = GenerateCandidatePoints(CenterPos, UnitCount, MinSafeDistance * (1.0f + RetryCount * 0.5f)); // 每次扩大50%范围
        ValidPoints = ValidatePointsOnNavMesh(Candidates, World);
    }

    // 5. 调整点间距（确保最终间距≥最小安全距离）
    AdjustPointDistances(ValidPoints, MinSafeDistance);

    // 6. 确保有效点数量与单位数量一致（不足时用最近点填充）
    if (ValidPoints.Num() > UnitCount)
    {
        ValidPoints.SetNum(UnitCount); // 截断多余的点
    }
    else if (ValidPoints.Num() < UnitCount)
    {
        // 极端情况：用中心位置填充（至少保证有目标点）
        while (ValidPoints.Num() < UnitCount)
        {
            ValidPoints.Add(CenterPos);
        }
    }
    
    // 7. 分配目标点给选中的单位
    for (int32 i = 0; i < CurrentlySelectedUnits.Num(); i++) {
        if (ARuleOfCharacter* Unit = Cast<ARuleOfCharacter>(CurrentlySelectedUnits[i]->GetOwner())) {
            if (ALowPolyGameState* TempState = GetWorld()->GetGameState<ALowPolyGameState>()) {
                FCharacterData CharacterData = TempState->GetCharacterData(Unit->GUID);
                CharacterData.LocationToMove = ValidPoints[i];
                UE_LOG(LogTemp, Log, TEXT("Task:LocationToMove已分配"));
                
            }
        }
    }  

    // 8. 调试：在所有点位生成标识物
    if (!GetWorld()) return;
    FRotator SpawnRotation = FRotator::ZeroRotator;  // 零旋转
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;  // 若碰撞则调整位置，无法调整则不生成

    //生成周围点位
    for (FVector Temp : ValidPoints) {
        FVector SpawnLocation = Temp;
        FVector SpawnScale = FVector(0.2f, 0.2f, 0.2f);
        FTransform SpawnTransform(SpawnRotation, SpawnLocation, SpawnScale);
        GetWorld()->SpawnActor<ARuleOfCharacter>(ATowers::StaticClass(), SpawnTransform, SpawnParams);
    }
    //生成点击位置
    FVector SpawnLocation = CenterPos;
    FVector SpawnScale = FVector(0.2f, 0.2f, 0.8f);
    FTransform SpawnTransform(SpawnRotation, SpawnLocation, SpawnScale);
    GetWorld()->SpawnActor<ARuleOfCharacter>(ATowers::StaticClass(), SpawnTransform, SpawnParams);

}

TArray<FVector> ASelectionManager::GenerateCandidatePoints(FVector TargetCenter, int32 UnitCount, float MinSafeDistance)
{
    TArray<FVector> Candidates;
    if (UnitCount <= 0) return Candidates;

    float BaseRadius = MinSafeDistance; // 初始半径
    int32 Remaining = UnitCount;
    int32 Layer = 0;

    while (Remaining > 0)
    {
        const float CurrentRadius = BaseRadius + Layer * MinSafeDistance; // 每层半径递增
        const float Circumference = 2 * PI * CurrentRadius;
        const int32 MaxPerLayer = FMath::Max(1, FMath::FloorToInt(Circumference / MinSafeDistance)); // 每层最大点数
        const int32 ThisLayerCount = FMath::Min(Remaining, MaxPerLayer);

        // 生成当前层的点（均匀角度分布，每层错开30度避免径向对齐）
        for (int32 i = 0; i < ThisLayerCount; i++)
        {
            const float Angle = (2 * PI * i / ThisLayerCount) + (Layer * PI / 6.0f);
            FVector Offset(
                CurrentRadius * FMath::Cos(Angle),
                CurrentRadius * FMath::Sin(Angle),
                0.0f
            );
            Candidates.Add(TargetCenter + Offset);
        }

        Remaining -= ThisLayerCount;
        Layer++;
    }

    return Candidates;
}

TArray<FVector> ASelectionManager::ValidatePointsOnNavMesh(const TArray<FVector>& Candidates, UWorld* World)
{
    TArray<FVector> ValidPoints;
    if (!World) return ValidPoints;

    if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(World))
    {
        for (const FVector& Candidate : Candidates)
        {
            FNavLocation NavLoc;
            // 投影到NavMesh（允许50单位误差）
            if (NavSys->ProjectPointToNavigation(Candidate, NavLoc, FVector(50.0f, 50.0f, 50.0f)))
            {
                ValidPoints.Add(NavLoc.Location);
            }
        }
    }

    return ValidPoints;
}

void ASelectionManager::AdjustPointDistances(TArray<FVector>& Points, float MinSafeDistance)
{
    const int32 Iterations = 3; // 迭代次数（平衡性能与精度）
    const float Repulsion = 8.0f; // 排斥力强度

    for (int32 It = 0; It < Iterations; It++)
    {
        for (int32 i = 0; i < Points.Num(); i++)
        {
            for (int32 j = i + 1; j < Points.Num(); j++)
            {
                FVector& P1 = Points[i];
                FVector& P2 = Points[j];
                const FVector Delta = P2 - P1;
                const float Dist = Delta.Size();

                if (Dist > 0 && Dist < MinSafeDistance)
                {
                    const float Push = (MinSafeDistance - Dist) * 0.5f * Repulsion;
                    const FVector Dir = Delta.GetSafeNormal();
                    P1 -= Dir * Push;
                    P2 += Dir * Push;
                }
            }
        }
    }
}

