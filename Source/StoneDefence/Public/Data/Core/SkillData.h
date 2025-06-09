#pragma once

#include "CoreMinimal.h"
#include "Data/Core/SkillDataCore.h"
#include "../StoneDefenceType.h"
#include "SkillData.generated.h"

class ARuleOfProjectile;

USTRUCT(BlueprintType)
struct FSkillData : public FSkillDataCore {
	GENERATED_BODY()
	typedef FSkillDataCore Super;

	FSkillData();
	virtual void Init();
public:

	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		FSkillType SkillType;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		UTexture2D* SkillIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		TSubclassOf<ARuleOfProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float GoldModify;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float HealthModify;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float PhysicalAttackModify;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float AttackSpeedModify;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float ArmorModify;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float CDModify;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float AttackRange;//��Ϊ�൥λ������0����ȫ�������е���
	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float MaxSkillDuration;
	UPROPERTY()
		float SkillDuration;


	UPROPERTY(EditDefaultsOnly, Category = "Skill Profit")
		float SkillCD;
	UPROPERTY()
		float CDTime;

};


