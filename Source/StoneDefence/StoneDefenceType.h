// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StoneDefenceType.generated.h"

UENUM() 
enum ETeam {
	RED,
	BLUE,
	NONE
};


UENUM(BlueprintType)
namespace EGameCharacterType{
	enum Type {
		MINI,
		BOSS,
		MAX
	};
}

UENUM(BlueprintType)
enum class EProjectileType :uint8 {
	PROJECTILE_LASER,//���⣬���赲����͸
	PROJECTILE_TRACKING,//��ͨ���ͣ����赲
	PROJECTILE_TRACKING2,
	PROJECTILE_BULLET,//����
	PROJECTILE_RANGE,//��Χ�˺�
	PROJECTILE_LIGHTING,//�׻��������˺�
	PROJECTILE_GRENADE,//���䣬��������
};

UENUM(BlueprintType)
enum class ECharacterType :uint8
{
	TOWER,
	MONSTER,
};


/*----------------------------------------------------------------------------------------------------*/
/*-------------------------------------------����ö��--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

UENUM(BlueprintType)
enum class ESkillTimeType : uint8 {//ESkillType -> ESkillTimeType
	BURST,
	SECTION,
	ITERATION,
};

UENUM(BlueprintType)
enum class ESkillTargetNumType : uint8 {//ESkillAttackNumType -> ESkillTargetNumType
	SINGLE,
	MULTIPLE,
};


UENUM(BlueprintType)
enum class ESkillBoostType : uint8 {//ESkillEffectType -> ESkillBoostType
	ADD,//��Ѫ
	SUBTRACT,//��Ѫ
};

UENUM(BlueprintType)
enum class ESkillTargetType : uint8 {
	ALLIES,
	ENEMY,
};

USTRUCT(BlueprintType)
struct FSkillType {
	GENERATED_BODY()

	FSkillType()
		:SkillTimeType(ESkillTimeType::SECTION),
		SkillTargetNumType(ESkillTargetNumType::MULTIPLE),
		SkillBoostType(ESkillBoostType::ADD),
		SkillTargetType(ESkillTargetType::ALLIES)
	{ }
	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillTimeType SkillTimeType;

	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillTargetNumType SkillTargetNumType;

	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillBoostType SkillBoostType;

	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillTargetType SkillTargetType;
};