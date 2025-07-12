// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/Skill/UI_SkillSlot.h"
#include "Global/UI_Data.h"


void UUI_SkillSlot::NativeConstruct() 
{
	Super::NativeConstruct();

	if (PlayerSkillNumberInData == 9) {
		PlayerSkillNumberInData = 0;//到9后重置为0并应用数字，这样技能表数字顺序就位1234567890，符合主流游戏技能顺序
		PlayerSkillNumber->SetText(FText::FromString(FString::FromInt(PlayerSkillNumberInData)));
	}
	else {
		PlayerSkillNumber->SetText(FText::FromString(FString::FromInt(++PlayerSkillNumberInData)));
	}

	KeyNumberForSKill = PlayerSkillNumberInData;
	//按键映射
	FString PlayerSkillName = FString::Printf(TEXT("PlayerSkill_%i"), PlayerSkillNumberInData);
	GetPlayerController()->InputComponent->BindAction(*PlayerSkillName, IE_Pressed, this, &UUI_SkillSlot::OnClickedWidget);
	//此处BindAction参数1需要解引用，是因为需要传入FName，而UE4不支持FString和FName的隐式转换（避免性能开销过大）
	//故这里调用FString在UE4内置的重载操作符operator*()，将PlayerSkillName转换为TCHAR* ，此时，便能够隐式转换为FName
}

void UUI_SkillSlot::OnClickedWidget() {
	if (GetPlayerState()->SkillVerification(GUID)) {
		GetPlayerState()->UsePlayerSkill(GUID);
	}
}


void UUI_SkillSlot::UpdateUI() {
	UpdateSlotUI(GetPlayerSkillData()->SkillIcon.LoadSynchronous(), GetPlayerSkillData()->SkillNumber);
}


FPlayerSkillData* UUI_SkillSlot::GetPlayerSkillData() {
	return GetPlayerState()->GetPlayerSkillData(GUID);
}


