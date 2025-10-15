#pragma once

#include "Engine.h"
#include "StoneDefence.h"

#define TD_PRINT_S(Format,...) \
	do{ \
	const FString Msg_s = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
		if(GEngine){ \
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, Msg_s); \
		} \
	}while(0)

#define TD_LOGPRINT(Type,Format,...) \
	do { \
		const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
		UE_LOG(LogStoneDefence, Type, TEXT("%s"), *Msg); \
	}while(0) \

#if WITH_EDITOR

#define TD_PRINT_R(Type,Format,...) \
	do{ \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
		if(GEngine){ \
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, Msg); \
		} \
		UE_LOG(LogStoneDefence, Type, TEXT("%s"), *Msg); \
	}while(0)

#else

#define TD_PRINT_R(Type,Format,...) \
	TD_LOGPRINT(Type,Format,##__VA_ARGS__)

#endif

//技能效果应用
#define SKILL_TABLE_ACQUISITION(BasicValue,ModifyValue) \
float Value = BasicValue; \
for (const auto& Temp : AdditionalSkillData) \
{ \
	switch (Temp.Value.SkillType.SkillBoostType) { \
		case ESkillBoostType::ADD: \
			Value += Temp.Value.ModifyValue; \
			break; \
		case ESkillBoostType::SUBTRACT: \
			Value -= Temp.Value.ModifyValue; \
			break; \
	} \
} \
return Value;

//技能效果应用（CD减免）
#define SKILL_TABLE_ACQUISITION_CD(SkillCD) \
float Value = SkillCD; \
	for (const auto& Temp : AdditionalSkillData) \
	{ \
		Value -= Temp.Value.CDModify; \
	} \
return Value;

#define GAMETHUMBNAIL_SCREENSHOT(InWidth,InHeight,InTexture,InOuter) (new RenderingUtils::FScreenShot(InWidth,InHeight,InTexture,InOuter))