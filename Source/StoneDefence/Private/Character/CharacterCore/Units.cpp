// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Units.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Core/GameCore/TD_GameState.h"

AUnits::AUnits() {
	PrimaryActorTick.bCanEverTick = true;

    // 创建Steering组件并附加到角色
    SteeringComponent = CreateDefaultSubobject<USteeringBehaviorComponent>(TEXT("SteeringComponent"));

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = 200.f; // 设置最大行走速度
		MovementComp->SetAvoidanceEnabled(false);
		//MovementComp->AvoidanceConsiderationRadius = 100.0f;
		//MovementComp->AvoidanceWeight = 1.0f;
	}
}

void AUnits::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	//速度设定
	ATD_GameState* TempGameState = GetWorld()->GetGameState<ATD_GameState>();

	if (TempGameState) {
		if (isFreezed != TempGameState->GetGameData().bTimeFreezed) {//当自身冻结状态和世界冻结状态不匹配，判断解or冻结
			//能执行进来就是不匹配
			if (isFreezed == true)
				GetCharacterMovement()->MaxWalkSpeed = 200.f;//解冻
			else
				GetCharacterMovement()->MaxWalkSpeed = 0.f;//冻结
			isFreezed = !isFreezed;//更新自身冻结状态
		}
	}

}