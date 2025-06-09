#include "Data/Core/SkillData.h"

FSkillData::FSkillData() {
	Init();
}

void FSkillData::Init() {
	Super::Init();

	GoldModify = 0.f;
	HealthModify = 0.f;
	PhysicalAttackModify = 0.f;
	AttackSpeedModify = 0.f;
	ArmorModify = 0.f;
	CDModify = 0.f;
	AttackRange = 0.f;//若为多单位攻击，0代表全场景所有敌人
	MaxSkillDuration = 0.f;
	SkillDuration = 0.f;

}
