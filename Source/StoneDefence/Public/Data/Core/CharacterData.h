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
	/*--------------------------------------������Դ-------------------------------------------*/
	/*----------------------------------------------------------------------------------------*/
	//��ɫʵ����ͼ
	UPROPERTY(EditDefaultsOnly, Category = "Table")
	TAssetSubclassOf<class ARuleOfCharacter> CharacterBlueprintKey;
	//��ɫͼƬ
	UPROPERTY(EditDefaultsOnly, Category = "Table")
	TAssetPtr<UTexture2D> Icon;



	/*----------------------------------------------------------------------------------------*/
	/*--------------------------------------��������-------------------------------------------*/
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
	/*--------------------------------------��������-------------------------------------------*/
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
	/*--------------------------------------����������------------------------------------------*/
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
	/*--------------------------------------��ɫ����------------------------------------------*/
	/*---------------------------------------------------------------------------------------*/

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TArray<int32> CharacterSkillID;
	//��ɫӵ�еļ�������
	UPROPERTY()
	TArray<FSkillData> CharacterSkill;


	//��ʩ��������ļ���
	UPROPERTY()
	TMap<FGuid, FSkillData> AdditionalSkillData;
	//TArray<FSkillData> AdditionalSkillData;
		


	//���»�ȡ��Ϊ������������Ӱ��Ĳ���ֵ
	float GetMaxHealth() const;
	float GetAttack() const;
	float GetArmor() const;
	float GetCD() const;
	float GetAttackSpeed() const;
};