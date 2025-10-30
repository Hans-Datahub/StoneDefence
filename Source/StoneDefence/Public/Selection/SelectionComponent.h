#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../StoneDefenceType.h"
#include "SelectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, bool, bIsSelected);

UCLASS()
class STONEDEFENCE_API USelectableComponent : public USceneComponent
{
    GENERATED_BODY()

    USelectableComponent();
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadOnly, Category = "Selection")
        bool bIsSelected = false;

    UPROPERTY(BlueprintAssignable, Category = "Selection")
        FOnSelectionChanged OnSelectionChanged;

    UFUNCTION(BlueprintCallable, Category = "Selection")
        void SetSelected(bool bSelected);
};