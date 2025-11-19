// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/RuleOfAnimInstance.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Core/GameCore/TD_GameState.h"
#include "Data/GameData.h"


URuleOfAnimInstance::URuleOfAnimInstance() 
	:IsDeath(false), HasAttackOrder(false), Speed(0.0f),CurrentTime(0.0f),isDelayFinished(true)
{

}

void URuleOfAnimInstance::NativeInitializeAnimation() {

}

void URuleOfAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	if (IsDelayFinished(DeltaSeconds)) {
		ARuleOfCharacter* RuleOfCharacter = Cast<ARuleOfCharacter>(TryGetPawnOwner());
		ATD_GameState* TempGameState = GetWorld()->GetGameState<ATD_GameState>();
		if (!TempGameState) return;
		FCharacterData& CharacterData = TempGameState->GetCharacterData(RuleOfCharacter->GUID);
		if (CharacterData.bIsDummy) return;

		if (RuleOfCharacter) {
			HasAttackOrder = RuleOfCharacter->Isattack;
 			IsDeath = RuleOfCharacter->IsDeath();
			if(IsDeath){
				HasAttackOrder = RuleOfCharacter->Isattack;
			}

			if (TempGameState) {
				if (TempGameState->GetGameData().bTimeFreezed == false)
					Speed = RuleOfCharacter->GetVelocity().Size();
				else
					Speed = 0.f;
			}

		}

	}
}

bool URuleOfAnimInstance::IsDelayFinished(float DeltaSeconds) {
	if (!isDelayFinished) {
		CurrentTime += DeltaSeconds;
		if (CurrentTime > 0.3f) {
			isDelayFinished = true;
		}
		return false;
	}
	return true;
}