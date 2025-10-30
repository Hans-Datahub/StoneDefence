// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/MilitiaAIController.h"
#include "Character/CharacterCore/Marine.h"
#include "../StoneDefenceUtils.h"
#include "EngineUtils.h"

AActor* AMilitiaAIController::FindTarget(){
	//如果目标不存在或者已死亡，才执行新一轮FIndTarget
	if (!Target.IsValid() || !Target->IsActive()) {
		//声明塔存放数组
		TArray<ARuleOfCharacter*>TargetMarineArray;
		TArray<ARuleOfCharacter*>TargetBOSSArray;

		for (TActorIterator<AMarine>it(GetWorld(), AMarine::StaticClass()); it; ++it) {
			AMarine* Marine = *it;
			if (Marine && Marine->IsActive()) {
				//若发现目标为塔，则放入目标塔数组；若为主塔，放入目标主塔数组
				if (Marine->GetType() == EGameCharacterType::Type::MINI) {
					TargetMarineArray.Add(Marine);
				}
				else if (Marine->GetType() == EGameCharacterType::Type::BOSS) {
					TargetBOSSArray.Add(Marine);

				}
			}
		}

		//----------------计算BOSS和小兵谁更近--------------//

		AMarine* ClosestBOSS = Cast<AMarine>(StoneDefenceUtils::FindMostClosedTarget(TargetBOSSArray, GetPawn()->GetActorLocation()));
		AMarine* ClosestMarine = Cast<AMarine>(StoneDefenceUtils::FindMostClosedTarget(TargetMarineArray, GetPawn()->GetActorLocation()));

		FVector BossDistance = FVector(99999,99999,99999);
		FVector MarineDistance = FVector::ZeroVector;
		if(ClosestBOSS)
			BossDistance = GetPawn()->GetActorLocation() - ClosestBOSS->GetActorLocation();
		if(ClosestMarine)
			MarineDistance = GetPawn()->GetActorLocation() - ClosestMarine->GetActorLocation();

		//若BOSS距离仍比较远，先打小兵
		if (BossDistance.Size() > MarineDistance.Size()) {
			return ClosestMarine;
		}
		//若BOSS较近，先打BOSS
		return ClosestBOSS;
	}
	//若当前目标存在且没死，则继续返回改目标
	return Target.Get();
}