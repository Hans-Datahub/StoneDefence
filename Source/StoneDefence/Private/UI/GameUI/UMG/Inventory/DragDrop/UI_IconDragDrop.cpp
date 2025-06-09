// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Inventory/DragDrop/UI_IconDragDrop.h"
#include "Components/Image.h"

void UUI_IconDragDrop::SetDragIcon(UTexture2D* ICON) {
	Icon->SetBrushFromTexture(ICON);
}