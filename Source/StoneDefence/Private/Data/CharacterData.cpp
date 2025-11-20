// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CharacterData.h"
#include "../StoneDefenceMacro.h"


FCharacterData::FCharacterData()
	:Super()
{
	Init();
}

void FCharacterData::Init() {
	Super::Init();
	Name = NAME_None;
	ID = INDEX_NONE;
	Level = 1;
	MaxHealth = 100.f;
	Health = MaxHealth;
	PhysicalAttack = 10.f;
	Armor = 10.f;
	MaxEmpiricalValue = 100.f;
	EmpiricalValue = MaxEmpiricalValue;
	CD = 2.f;
	AttackSpeed = 0.66f;
	Gold = 80.f;
	MoveSpeed = 356.f;

	AddGold = 30.f;
	AddHealth = 50.f;
	AddPhysicalAttack = 10.f;
	AddArmor = 10.f;
	AddEmpiricalValue = 100;
	AddAttackSpeed = 0.01f;
	RestoreHealth = 0.2f;

	HealthLimitEnhanced = 0.f;
	HealthRecovery = 0.f;
	AttackEnhanced = 0.f;
	ArmorEnhanced = 0.f;
	AttackSpeedEnhanced = 0.f;
	CDReduce = 0.45f;

	Location = FVector::ZeroVector;
	Rotator = FRotator::ZeroRotator; 
	LocationToMove = FVector::ZeroVector;
	LastLocationToMove = FVector::ZeroVector;
	Team = ETeam::RED;
}

void FCharacterData::UpdateHealth() {
	Health = GetMaxHealth();
}

bool FCharacterData::UpdateExp(float InExp) {
	EmpiricalValue += InExp;
	if (EmpiricalValue >= MaxEmpiricalValue) {
		UpdateLevel();
		return true;
	}
	return false;
}

void FCharacterData::UpdateLevel() {
	//此处不直接设零是因为经验值可能会超出最大，需要留下超出的部分
	EmpiricalValue = EmpiricalValue - MaxEmpiricalValue;

	//升级后的属性处理
	float CoEfficient = .1f;
	Level++;
	MaxHealth += (Level - 1) * AddHealth * CoEfficient;
	AttackSpeed += (Level - 1) * AddAttackSpeed * CoEfficient;
	PhysicalAttack += (Level - 1) * AddPhysicalAttack * CoEfficient;
	Armor += (Level - 1) * AddArmor * CoEfficient;
	MaxEmpiricalValue += (Level - 1) * AddEmpiricalValue * CoEfficient;
	RestoreHealth += (RestoreHealth * Level) / 100;
	Gold += (Level - 1) * AddGold * CoEfficient;

	//升级后的奖励
	Health = GetMaxHealth();//升级后补满血
}

float FCharacterData::GetExpPercentage()const {
	if (MaxEmpiricalValue) {
		return EmpiricalValue / MaxEmpiricalValue;
	}
	return 0.0f;
}


float FCharacterData::GetMaxHealth() const {
	return MaxHealth;
	//SKILL_TABLE_ACQUISITION(Health, HealthModify);
}

float FCharacterData::GetAttack() const {
	return PhysicalAttack;
	//SKILL_TABLE_ACQUISITION(PhysicalAttack, PhysicalAttackModify);
}

float FCharacterData::GetArmor() const {
	return Armor;
	//SKILL_TABLE_ACQUISITION(Armor, ArmorModify);
}
float FCharacterData::GetCD() const {
	return CD;
	//SKILL_TABLE_ACQUISITION_CD(CD);
}
float FCharacterData::GetAttackSpeed() const {
	return AttackSpeed;
	//SKILL_TABLE_ACQUISITION(AttackSpeed, AttackSpeedModify);
}
float FCharacterData::GetMoveSpeed() const {
	return MoveSpeed;
	//SKILL_TABLE_ACQUISITION(MoveSpeed, MoveSpeedModify);

}