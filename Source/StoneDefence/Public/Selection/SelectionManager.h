#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StoneDefenceType.h"
#include "Selection/SelectionComponent.h"
#include "SelectionManager.generated.h"

UCLASS()
class STONEDEFENCE_API ASelectionManager : public AActor
{
    GENERATED_BODY()

public:
    ASelectionManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 输入处理
    void StartSelection(const FVector2D& ScreenPosition);
    void UpdateSelection(const FVector2D& CurrentScreenPosition);
    void FinishSelection();

    // 模式设置
    void SetSelectionMode(ELowpolySelectionMode NewMode) { CurrentSelectionMode = NewMode; }

    TArray<USelectableComponent*> GetUnitsInSelectionArea() const;
    void ApplySelection(const TArray<USelectableComponent*>& NewSelection);
    void ToggleSelectionMark(USelectableComponent* Unit, bool bVisible);
    void ClearAllSelections();
    void UpdateSelectionBoxVisual();
    void CreateSelectionBoxWidget();


    // 选择状态
    bool bIsSelecting = false;

    UPROPERTY(BlueprintReadOnly, Category = "Selection")
    FVector2D SelectionStart;
    UPROPERTY(BlueprintReadOnly, Category = "Selection")
    FVector2D SelectionEnd;

    FVector2D TunedStart;
    FVector2D TunedEnd;

    // 选择模式
    ELowpolySelectionMode CurrentSelectionMode = ELowpolySelectionMode::Normal;

    // 当前选中的单位
    UPROPERTY()
        TArray<USelectableComponent*> CurrentlySelectedUnits;

    // 框选可视化
    UPROPERTY()
        class UUserWidget* SelectionBoxWidget = nullptr;

};