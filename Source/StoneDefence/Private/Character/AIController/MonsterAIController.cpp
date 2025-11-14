// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AIController/MonsterAIController.h"
#include "Character/CharacterCore/Towers.h"
#include "../StoneDefenceUtils.h"
#include "EngineUtils.h"


AActor* AMonsterAIController::FindTarget(){
	//如果目标不存在或者已死亡，才执行新一轮FIndTarget
	if (!Target.IsValid() || !Target->IsActive()) {
		//声明塔存放数组
		TArray<ARuleOfCharacter*>MainTargetArray;
		TArray<ARuleOfCharacter*>NormalTargetArray;

		for (TActorIterator<ARuleOfCharacter>it(GetWorld(), ARuleOfCharacter::StaticClass()); it; ++it) {
			ARuleOfCharacter* TheCharacter = *it;
			if (TheCharacter && TheCharacter->IsActive()) {
				//若发现目标为塔，则放入目标塔数组；若为主塔，放入目标主塔数组
				if (TheCharacter->GetType() == EGameCharacterType::Type::MINI) {
					NormalTargetArray.Add(TheCharacter);
				}
				else if (TheCharacter->GetType() == EGameCharacterType::Type::BOSS) {
					MainTargetArray.Add(TheCharacter);

				}
			}
		}

		ARuleOfCharacter* MainTarget = Cast<ARuleOfCharacter>(StoneDefenceUtils::FindMostClosedTarget(MainTargetArray, GetPawn()->GetActorLocation()));
		ARuleOfCharacter* NormalTarget = Cast<ARuleOfCharacter>(StoneDefenceUtils::FindMostClosedTarget(NormalTargetArray, GetPawn()->GetActorLocation()));

		//优先攻击主塔，其次普通塔
		if (MainTarget) { return MainTarget; }
		return NormalTarget;
	}
	return Target.Get();
}
	

void AMonsterAIController::AttackTarget(class ARuleOfCharacter* AttackTarget) {

}
