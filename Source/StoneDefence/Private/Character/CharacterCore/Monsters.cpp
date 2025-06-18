// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Monsters.h"
#include "GameFrameWork/CharacterMovementComponent.h"

void AMonsters::BeginPlay() {
	Super::BeginPlay();
}

void AMonsters::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//速度设定
/*	if (GetCharacterData().IsValid()) {
		if (GetCharacterData().GetWalkSpeed() != GetCharacterMovement()->MaxWalkSpeed) {
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterData().GetMoveSpeed();
		}
	}*/
	//这一部分加上，角色会停止移动，后面再改

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