#pragma once

#include "CoreMinimal.h"
#include "Data/Core/SkillData.h"
//#include "../StoneDefenceType.h"
#include "PlayerSkillData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSkillData : public FSkillData {
	GENERATED_BODY()

	FPlayerSkillData();

	virtual void Init();

	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
	float SkillNumber;
};
