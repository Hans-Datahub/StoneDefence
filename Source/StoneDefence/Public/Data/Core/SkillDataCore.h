#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "SkillData.h"
#include "SkillDataCore.generated.h"

USTRUCT(BlueprintType)
struct FSkillDataCore : public FTableRowBase {
	GENERATED_BODY()

	FSkillDataCore();

	virtual void Init();

	virtual bool IsValid() const;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Base Attribute")
		FName SkillName;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Base Attribute")
		int32 SkillID;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Base Attribute")
		int32 PriceInGold;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Base Attribute")
		int32 PriceInCopper;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Base Attribute")
		int32 PriceInDiamond;
};

