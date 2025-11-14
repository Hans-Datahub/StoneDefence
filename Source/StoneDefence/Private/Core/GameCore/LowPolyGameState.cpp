// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/LowPolyGameState.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/Anim/RuleOfAnimInstance.h"
#include "../StoneDefenceUtils.h"

ALowPolyGameState::ALowPolyGameState() {
	//角色配置表路径查找与赋值
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Militia(TEXT("/Game/GameData/MilitiaData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> MyTable_Marine(TEXT("/Game/GameData/MarineData"));

	AIMilitiaCharacterData = MyTable_Militia.Object;
	AIMarineCharacterData = MyTable_Marine.Object;
}

void ALowPolyGameState::BeginPlay() {
	AssignExistedUnit();
}

void ALowPolyGameState::AssignExistedUnit() {
	//获取所有在场单位
	TArray<AMilitia*> ExistedMilitiaArray = StoneDefenceUtils::GetAllActor<AMilitia>(GetWorld());
	TArray<AMarine*> ExistedMarineArray = StoneDefenceUtils::GetAllActor<AMarine>(GetWorld());

	//获取所有单位基础数据 , 以MilitiaTable中的数据为模板
	TArray<FCharacterData*> Datas;
	UDataTable* InCharacterData = AIMarineCharacterData;
	InCharacterData->GetAllRows(TEXT("CharacterData"), Datas);
	auto GetCharacterData = [&](int32 ID) ->FCharacterData* {
		for (auto& Temp : Datas) {
			if (Temp->ID == ID) {
				return Temp;
			}
		}
		return nullptr;
	};
	FCharacterData* CharacterData = GetCharacterData(-1);


	//注册所有Militia
	if (ExistedMilitiaArray.Num())
		for (AMilitia* Militia : ExistedMilitiaArray)
			AddCharacterData(Militia->GUID, *CharacterData);
	//注册所有Marine
	if (ExistedMarineArray.Num())
		for (AMarine* Marine : ExistedMarineArray)
			AddCharacterData(Marine->GUID, *CharacterData);
}

const TArray<FCharacterData*>& ALowPolyGameState::GetMilitiaDataArrayFromTable() {
	if (!CacheMilitiaDatas.Num()) //若缓存内无数据，直接读取行
		AIMilitiaCharacterData->GetAllRows(TEXT("Character Data"), CacheMilitiaDatas);
	return CacheMilitiaDatas;
}

const TArray<FCharacterData*>& ALowPolyGameState::GetMarineDataArrayFromTable() {
	if (!CacheMarineDatas.Num()) //若缓存内无数据，直接读取行
		AIMarineCharacterData->GetAllRows(TEXT("Character Data"), CacheMarineDatas);
	return CacheMarineDatas;
}