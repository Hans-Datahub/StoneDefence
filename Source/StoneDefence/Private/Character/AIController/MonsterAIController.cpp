// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/MonsterAIController.h"
#include "Character/CharacterCore/Towers.h"
#include "../StoneDefenceUtils.h"
#include "EngineUtils.h"


AActor* AMonsterAIController::FindTarget() {
	//如果目标不存在或者已死亡，才执行新一轮FIndTarget
	if (!Target.IsValid() || !Target->IsActive()) {
		//声明塔存放数组
		TArray<ARuleOfCharacter*>TargetMainTowerArray;
		TArray<ARuleOfCharacter*>TargetTowersArray;

		for (TActorIterator<ATowers>it(GetWorld(), ATowers::StaticClass()); it; ++it) {
			ATowers* TheCharacter = *it;
			if (TheCharacter && TheCharacter->IsActive()) {
				//若发现目标为塔，则放入目标塔数组；若为主塔，放入目标主塔数组
				if (TheCharacter->GetType() == EGameCharacterType::Type::MINI) {
					TargetTowersArray.Add(TheCharacter);
				}
				else if (TheCharacter->GetType() == EGameCharacterType::Type::BOSS) {
					TargetMainTowerArray.Add(TheCharacter);

				}
			}
		}

		ATowers* MainTowers = Cast<ATowers>(StoneDefenceUtils::FindMostClosedTarget(TargetMainTowerArray, GetPawn()->GetActorLocation()));
		ATowers* NormalTowers = Cast<ATowers>(StoneDefenceUtils::FindMostClosedTarget(TargetTowersArray, GetPawn()->GetActorLocation()));

		//优先攻击主塔，其次普通塔
		if (MainTowers) { return MainTowers; }
		return NormalTowers;
	}
	return Target.Get();
}
	

void AMonsterAIController::AttackTarget(class ARuleOfCharacter* AttackTarget) {

}
