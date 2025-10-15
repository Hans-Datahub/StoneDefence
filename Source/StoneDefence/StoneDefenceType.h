// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StoneDefenceType.generated.h"


UENUM(BlueprintType)
enum class ESubmissionSkillRequestType :uint8
{
	AUTO,     //生成权利交由服务端
	MANUAL,   //生成权利交由客户端
};

UENUM(BlueprintType)
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
	PROJECTILE_NONE,
	PROJECTILE_LASER,//激光，无阻挡，穿透
	PROJECTILE_TRACKING,//普通类型，可阻挡
	PROJECTILE_TRACKING2,
	PROJECTILE_BULLET,//跟踪
	PROJECTILE_RANGE,//范围伤害
	PROJECTILE_LIGHTING,//雷击，链条伤害
	PROJECTILE_GRENADE,//抛射，类似手雷
};

UENUM(BlueprintType)
enum class ECharacterType :uint8
{
	TOWER,
	MONSTER,
};


/*----------------------------------------------------------------------------------------------------*/
/*-------------------------------------------技能枚举--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

UENUM(BlueprintType)
enum class ESkillTimeType : uint8 {//ESkillType -> ESkillTimeType
	BURST,
	SECTION,
	ITERATION,
};

UENUM(BlueprintType)
enum class ESkillBoostType : uint8 {//ESkillEffectType -> ESkillBoostType
	ADD,//加血
	SUBTRACT,//减血
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
		SkillBoostType(ESkillBoostType::ADD),
		SkillTargetType(ESkillTargetType::ALLIES)
	{ }
	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillTimeType SkillTimeType;

	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillBoostType SkillBoostType;

	UPROPERTY(EditDefaultsOnly, Category = "SkillType")
		ESkillTargetType SkillTargetType;
};

struct FMapSize
{
	FVector2D BigMapRealSize;
};

UENUM(BlueprintType)
enum class EMiniMapType :uint8
{
	REAL_MAP,
	GRAPHICAL_MAP,
};

enum EGameSaveType
{
	NOSAVE = 0b0000,
	ARCHIVES = 0b0001,
};