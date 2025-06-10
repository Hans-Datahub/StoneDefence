// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "Engine/DataTable.h"
#include "Data/Core/SkillData.h"
//#include "Data/Core/SkillDataCore.h"
#include "CharacterData.generated.h"



USTRUCT(BlueprintType)
struct FCharacterData : public FSkillDataCore {
	GENERATED_BODY()
	typedef FSkillDataCore Super;
public:
	FCharacterData();
	void Init();
	void UpdateHealth();
	bool UpdateExp(float InExp);
	void UpdateLevel();
	float GetExpPercentage()const;

	/*----------------------------------------------------------------------------------------*/
	/*--------------------------------------基础资源-------------------------------------------*/
	/*----------------------------------------------------------------------------------------*/
	//角色实例蓝图
	UPROPERTY(EditDefaultsOnly, Category = "Table")
	TAssetSubclassOf<class ARuleOfCharacter> CharacterBlueprintKey;
	//角色图片
	UPROPERTY(EditDefaultsOnly, Category = "Table")
	TAssetPtr<UTexture2D> Icon;



	/*----------------------------------------------------------------------------------------*/
	/*--------------------------------------基础属性-------------------------------------------*/
	/*----------------------------------------------------------------------------------------*/
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		FName Name;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		int32 ID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attribute")
		int32 Level;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float PhysicalAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float Armor;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float MaxEmpiricalValue;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float EmpiricalValue;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float CD;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AttackSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float Gold;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		FText Introduction;



	/*----------------------------------------------------------------------------------------*/
	/*--------------------------------------升级增益-------------------------------------------*/
	/*----------------------------------------------------------------------------------------*/
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AddGold;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AddHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AddPhysicalAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AddArmor;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AddEmpiricalValue;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float AddAttackSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
		float RestoreHealth;



	/*----------------------------------------------------------------------------------------*/
	/*--------------------------------------防御塔属性------------------------------------------*/
	/*----------------------------------------------------------------------------------------*/
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		float HealthLimitEnhanced; //AddPassiveSkillHealth->HealthLimitEnhanced
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		float HealthRecovery; //AddContinueHealth
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		float AttackEnhanced;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		float ArmorEnhanced;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		float AttackSpeedEnhanced;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		float CDReduce;


	UPROPERTY()
		FVector Location;
	UPROPERTY()
		FRotator Rotator;
	TEnumAsByte<ETeam> Team;



	/*---------------------------------------------------------------------------------------*/
	/*--------------------------------------角色技能------------------------------------------*/
	/*---------------------------------------------------------------------------------------*/
	//角色拥有的技能数据
	UPROPERTY()
	TArray<FSkillData> CharacterSkill;


	//被施法至自身的技能
	UPROPERTY()
	TMap<FGuid, FSkillData> AdditionalSkillData;
	//TArray<FSkillData> AdditionalSkillData;
		


	//以下获取的为经过被动技能影响的参数值
	float GetMaxHealth() const;
	float GetAttack() const;
	float GetArmor() const;
	float GetCD() const;
	float GetAttackSpeed() const;
};