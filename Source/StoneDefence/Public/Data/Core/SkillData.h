#pragma once

#include "CoreMinimal.h"
#include "Data/Core/SkillDataCore.h"
#include "../StoneDefenceType.h"
#include "Character/Projectile/RuleOfProjectile.h"
#include "SkillData.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		FText SkillIntroduction;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		bool bSkillEffected;// = bBecomeEffective
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		ESubmissionSkillRequestType SubmissionSkillRequestType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		float MaxSkillDuration;
	UPROPERTY()
		float SkillDuration;

	UPROPERTY()//进行技能计时，随时间累加，每秒重置一次
		float SkillCountTime;//SkillDuratoinTime -> SkillCountTime

	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		float SkillCD;
	UPROPERTY(EditDefaultsOnly, Category = "Skill Attribute")
		float MaxSkillCD; // = CDTime


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
		float AttackRange;//若为多单位攻击，0代表全场景所有敌人


	float GetDurationPercentage() const;
	float GetSkillCDPercentage() const;// = GetCDPercent()
	void ResetDuration();
	void ResetCD();

	friend bool operator==(const FSkillData& X, const FSkillData& Y) {
		return X.SkillID == Y.SkillID;
	}
};


