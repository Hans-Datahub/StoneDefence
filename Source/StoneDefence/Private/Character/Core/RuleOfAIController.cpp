// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/RuleOfAIController.h"

void ARuleOfAIController::AttackTarget(class ARuleOfCharacter* AttackTarget) {

}

void ARuleOfAIController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (GetLocalRole() == ROLE_Authority){//此处Role被UE5移入Private，使用GetLocalRole()替代
		if (ARuleOfCharacter* InCharacter = Cast<ARuleOfCharacter>(GetPawn())) {
			FCharacterData& Data = InCharacter->GetCharacterData();
			if (Data.IsValid()) {
				Data.Location = InCharacter->GetActorLocation();
				Data.Rotator = InCharacter->GetActorRotation();
			}
		}
	}
}