// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIController/TowerAIController.h"
#include "Character/CharacterCore/Towers.h"
#include "Character/CharacterCore/Monsters.h"
#include "../StoneDefenceUtils.h"

ATowerAIController::ATowerAIController()
	:HeartbeatDiagnosis(0.0f)
{

}


void ATowerAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	HeartbeatDiagnosis += DeltaTime;
	if (HeartbeatDiagnosis >= 0.5f) {
		BTService_FindTarget();
		HeartbeatDiagnosis = 0;
	}

	if (TArrayMonsters.Num()) {
		if (ATowers* Tower = GetPawn<ATowers>()) {
			if (!Target.IsValid() || !Target->IsActive()) {//若目标无效或者死亡，则从新寻找目标
				Target = Cast<ARuleOfCharacter>(FindTarget());
			}
			if(Target.IsValid()) {
				Tower->TowerRotation = FRotationMatrix::MakeFromX(Target->GetActorLocation() - GetPawn()->GetActorLocation()).Rotator();
				if (GetPawn()->GetActorRotation() != FRotator::ZeroRotator) {//旋转重新归零
					Tower->TowerRotation -= GetPawn()->GetActorRotation();
				}
			}
		}
	}
}

AActor* ATowerAIController::FindTarget() {
	if (TArrayMonsters.Num()) {
		return StoneDefenceUtils::FindMostClosedTarget(TArrayMonsters, GetPawn()->GetActorLocation());
	}
	return nullptr;
}

void ATowerAIController::BTService_FindTarget() {
	TArrayMonsters.Empty();

	if (ATowers* Tower = GetPawn<ATowers>()) {
		if (Tower->IsActive()) {
			for (TActorIterator<AMonsters>it(GetWorld(), AMonsters::StaticClass()); it; ++it) {
				if (AMonsters* TheCharacter = *it) {
					if (TheCharacter->IsActive()) {
						FVector TMDistance = TheCharacter->GetActorLocation() - GetPawn()->GetActorLocation();
						if (TMDistance.Size() <= 1600) {
							TArrayMonsters.Add(TheCharacter);
						}
					}
				}
			}

			AttackTarget(Target.Get());
		}
	}
}


void ATowerAIController::AttackTarget(class ARuleOfCharacter* AttackTarget) {

	if (ATowers* Tower = GetPawn<ATowers>()) {
		//若队列中有目标
		if (TArrayMonsters.Num() > 0) {
			Tower->Isattack = true;
		}
		else {
			Tower->Isattack = false;
		}
	}
}
