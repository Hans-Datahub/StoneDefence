#include "Data/Core/SkillData.h"

FSkillData::FSkillData() {
	Init();
}

void FSkillData::Init() {
	Super::Init();

	//技能效果
	GoldModify = 0.f;
	HealthModify = 0.f;
	PhysicalAttackModify = 0.f;
	AttackSpeedModify = 0.f;
	ArmorModify = 0.f;
	CDModify = 60.f;
	MoveSpeedModify = 0.0f;

	//0代表技能作用于全场景所有敌人
	AttackRange = 0.f;

	SubmissionSkillRequestType = ESubmissionSkillRequestType::AUTO;

	//技能是否发挥过作用
	bSkillEffected = false;

	//技能持续时间
	MaxSkillDuration = 5.f;
	SkillDuration = 0.f;
	//技能CD
	SkillCD = 0.f;
	MaxSkillCD = 0.f;

}

float FSkillData::GetDurationPercentage() const
{
	return MaxSkillDuration == 0.f ? 0 : (SkillDuration / MaxSkillDuration);
}

float FSkillData::GetSkillCDPercentage() const {
	return MaxSkillCD == 0.f ? 0 : (SkillCD / MaxSkillCD);
}

void FSkillData::ResetDuration()
{
	SkillDuration = MaxSkillDuration;
}

void FSkillData::ResetCD(){
	SkillCD = MaxSkillCD;
}
