#include "Selection/SelectionComponent.h"

USelectableComponent::USelectableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USelectableComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USelectableComponent::SetSelected(bool bSelected)
{
    if (bIsSelected == bSelected)
        return;

    bIsSelected = bSelected;
    OnSelectionChanged.Broadcast(bIsSelected);

}