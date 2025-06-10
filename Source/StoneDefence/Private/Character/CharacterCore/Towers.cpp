// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCore/Towers.h"
#include "Components/StaticMeshComponent.h"
#include "DestructibleComponent.h"
#include "Particles/ParticleSystemComponent.h"

ATowers::ATowers() {
	ParticleMesh = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleMesh"));
	StaticMeshBuilding = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangeBuilding"));
	DestructibleMeshBuilding = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Substitute"));

	ParticleMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshBuilding->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DestructibleMeshBuilding->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

float ATowers::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController*EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;  
}

EGameCharacterType::Type ATowers::GetCharacterType() {
	return EGameCharacterType::Type::MINI;
}

ETeam ATowers::GetTeamType() {
	return ETeam::RED;
}

void ATowers::RegisterTeam() {
	GetCharacterData().Team == ETeam::RED;
}

void ATowers::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) {
	Super::OnClicked(TouchedComponent, ButtonPressed);

	//if (ClickedTower) {
	//	ClickedTower = nullptr;
	//}
	//else {
	//	ClickedTower = this;
	//}
}