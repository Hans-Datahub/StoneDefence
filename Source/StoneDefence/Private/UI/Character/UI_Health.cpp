// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/UI_Health.h"



void UUI_Health::NativeConstruct() {
	Super::NativeConstruct();
}

void UUI_Health::SetTitle(const FString& Msg) {
	Title->SetText(FText::FromString(Msg));
}

void UUI_Health::SetHealth(float HealthValue) {
	Health->SetPercent(HealthValue);
}
void UUI_Health::AddTakenSkillSlot(const FGuid& TakenSkillID) {
	if (TakenSkillSlotClass) {
		if (UUI_TakenSkillSlot* SkillSlot = CreateWidget<UUI_TakenSkillSlot>(GetWorld(), TakenSkillSlotClass)) {
			SkillSlot->GUID = TakenSkillID;
			if (SkillSlot->GetSkillData().IsValid()) {
				UTexture2D* SkillICON = SkillSlot->GetSkillData().SkillIcon.LoadSynchronous();
				SkillSlot->SetTexture(SkillICON);
				TakenSkillList->AddChild(SkillSlot);
			}			
		}
	}
}

bool UUI_Health::RemoveTakenSkillSlot(const FGuid& TakenSkillID) {
	UUI_TakenSkillSlot* RemoveSlot = nullptr;
	for (UPanelSlot* PanelSlot : TakenSkillList->GetSlots()) {
		if (UUI_TakenSkillSlot* SkillSlot = Cast<UUI_TakenSkillSlot>(PanelSlot->Content)) {
			if (SkillSlot->GUID == TakenSkillID) {
				RemoveSlot = SkillSlot;
				break;
			}
		}
	}
	return TakenSkillList->RemoveChild(RemoveSlot);
}
