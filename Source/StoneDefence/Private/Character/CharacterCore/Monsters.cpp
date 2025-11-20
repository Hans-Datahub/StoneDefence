// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Monsters.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Core/GameCore/TD_GameState.h"


void AMonsters::BeginPlay() {
	Super::BeginPlay();

	//速度初始化
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = 600.f; // 设置最大行走速度
		MovementComp->SetAvoidanceEnabled(true);
		MovementComp->AvoidanceConsiderationRadius = 100.0f;
		MovementComp->AvoidanceWeight = 1.0f;
	}
	isFreezed = false;
}

void AMonsters::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//速度设定
	ATD_GameState* TempGameState = GetWorld()->GetGameState<ATD_GameState>();

	if (TempGameState) {
		if (isFreezed != TempGameState->GetGameData().bTimeFreezed){//当自身冻结状态和世界冻结状态不匹配，判断解or冻结
			//能执行进来就是不匹配
			if (isFreezed == true)
				GetCharacterMovement()->MaxWalkSpeed = 600.f;//解冻
			else
				GetCharacterMovement()->MaxWalkSpeed = 0.f;//冻结
			isFreezed = !isFreezed;//更新自身冻结状态
		}	
	}
}

EGameCharacterType::Type AMonsters::GetCharacterType() {
	return EGameCharacterType::Type::MINI;
}

ETeam AMonsters::GetTeamType() {
	return ETeam::BLUE;
}

void AMonsters::RegisterTeam() {
	GetCharacterData().Team == ETeam::BLUE;
}

void AMonsters::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) {
	Super::OnClicked(TouchedComponent, ButtonPressed);

	//if (ClickedMonster) {
	//	ClickedMonster = nullptr;
	//	UE_LOG(LogTemp, Warning, TEXT("nullptr"));
	//}
	//else {
	//	ClickedMonster = this;
	//	UE_LOG(LogTemp, Warning, TEXT("this"));
	//}
}