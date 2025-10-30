#include "Selection/SelectionManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Character/Core/RuleOfCharacter.h"

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


