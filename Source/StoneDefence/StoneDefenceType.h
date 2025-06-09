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
	PROJECTILE_LASER,//¼¤¹â£¬ÎÞ×èµ²£¬´©Í¸
	PROJECTILE_TRACKING,//ÆÕÍ¨ÀàÐÍ£¬¿É×èµ²
	PROJECTILE_TRACKING2,
	PROJECTILE_BULLET,//¸ú×Ù
	PROJECTILE_RANGE,//·¶Î§ÉËº¦
	PROJECTILE_LIGHTING,//À×»÷£¬Á´ÌõÉËº¦
	PROJECTILE_GRENADE,//Å×Éä£¬ÀàËÆÊÖÀ×
};

UENUM(BlueprintType)
enum class ECharacterType :uint8
{
	TOWER,
	MONSTER,
};


/*----------------------------------------------------------------------------------------------------*/
/*-------------------------------------------¼¼ÄÜÃ¶¾Ù--------------------------------------------------*/
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
	ADD,//¼ÓÑª
	SUBTRACT,//¼õÑª
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