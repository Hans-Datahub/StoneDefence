#include "UI/GameUI/UMG/Skill/UI_TakenSkillSlot.h"

void UUI_TakenSkillSlot::NativeConstruct() {
	Super::NativeConstruct();

	DynamicCDMaterial = TakenSkillCD->GetDynamicMaterial();
}

void UUI_TakenSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	DrawTakenSkillCD(InDeltaTime);
}



FSkillData& UUI_TakenSkillSlot::GetSkillData() {
	return GetGameState()->GetSkillData(GUID);
}

void UUI_TakenSkillSlot::SetTexture(UTexture2D* InTexture) {
	TakenSkillBackGround->SetBrushFromTexture(InTexture);
}

void UUI_TakenSkillSlot::DrawTakenSkillCD(float InTakenSkillCD) {
	if (DynamicCDMaterial) {
		if (InTakenSkillCD > 0.0f && InTakenSkillCD < 1.0f) {
			DynamicCDMaterial->SetScalarParameterValue("Transparency", true);
			TakenSkillCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			DynamicCDMaterial->SetScalarParameterValue("Transparency", false);
			TakenSkillCD->SetVisibility(ESlateVisibility::Hidden);
		}
		DynamicCDMaterial->SetScalarParameterValue("Percentage", InTakenSkillCD);//������ͼ�У�CD���ʵİٷֱ�
	}
}