// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/MonsterAIController.h"
#include "Character/CharacterCore/Towers.h"
#include "../StoneDefenceUtils.h"
#include "EngineUtils.h"


AActor* AMonsterAIController::FindTarget() {
	//���Ŀ�겻���ڻ�������������ִ����һ��FIndTarget
	if (!Target.IsValid() || !Target->IsActive()) {
		//�������������
		TArray<ARuleOfCharacter*>TargetMainTowerArray;
		TArray<ARuleOfCharacter*>TargetTowersArray;

		for (TActorIterator<ATowers>it(GetWorld(), ATowers::StaticClass()); it; ++it) {
			ATowers* TheCharacter = *it;
			if (TheCharacter && TheCharacter->IsActive()) {
				//������Ŀ��Ϊ���������Ŀ�������飻��Ϊ����������Ŀ����������
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

		//���ȹ��������������ͨ��
		if (MainTowers) { return MainTowers; }
		return NormalTowers;
	}
	return Target.Get();
}
	

void AMonsterAIController::AttackTarget(class ARuleOfCharacter* AttackTarget) {

}
