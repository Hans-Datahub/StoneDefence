#include "Data/Core/SkillDataCore.h"


FSkillDataCore::FSkillDataCore() {
	Init();
}


void FSkillDataCore::Init() {
	SkillName = NAME_None;
	SkillID = INDEX_NONE;
	PriceInGold = 0;
	PriceInDiamond = 0;
	PriceInCopper = 0;
}

bool FSkillDataCore::IsValid() const {
	return SkillID != INDEX_NONE;
}