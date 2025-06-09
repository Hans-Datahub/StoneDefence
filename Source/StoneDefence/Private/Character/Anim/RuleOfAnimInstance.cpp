// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/RuleOfAnimInstance.h"
#include "Character/Core/RuleOfCharacter.h"


URuleOfAnimInstance::URuleOfAnimInstance() 
	:IsDeath(false), IsAttack(false), Speed(0.0f),CurrentTime(0.0f),isDelayFinished(true)
{

}

void URuleOfAnimInstance::NativeInitializeAnimation() {

}

void URuleOfAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	if (IsDelayFinished(DeltaSeconds)) {
		ARuleOfCharacter* RuleOfCharacter = Cast<ARuleOfCharacter>(TryGetPawnOwner());
		if (RuleOfCharacter) {
			IsAttack = RuleOfCharacter->Isattack;
			Speed = RuleOfCharacter->GetVelocity().Size();
			IsDeath = !(RuleOfCharacter->IsActive());
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