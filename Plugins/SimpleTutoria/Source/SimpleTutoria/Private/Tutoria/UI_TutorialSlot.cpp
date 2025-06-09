// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutoria/UI_TutorialSlot.h"
#include "Components/Button.h"

UUI_TutorialSlot::UUI_TutorialSlot(const FObjectInitializer& ObjectInitializer) 
	:Super(ObjectInitializer)
{
	Index = INDEX_NONE;
}

void UUI_TutorialSlot::NativeConstruct() {
	Super::NativeConstruct();
	PlayButton->OnClicked.AddDynamic(this, &UUI_TutorialSlot::Play);

}

bool UUI_TutorialSlot::IsIndexValid() {
	return Index != INDEX_NONE;
}


void UUI_TutorialSlot::Play() {
	if (SimpleTutoriaMulticastDelegate.IsBound()) {
		if (SimpleTutoriaMulticastDelegate.Execute(Index)) {

		}
	}
}
