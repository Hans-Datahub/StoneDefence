#include "Data/Core/PlayerSkillData.h"

FPlayerSkillData::FPlayerSkillData()
	:FSkillData()
{

}

void FPlayerSkillData::Init() {
	FSkillData::Init();
	SkillNumber = 0;
}

bool FPlayerSkillData::IsValid() {
	return Super::IsValid();
}