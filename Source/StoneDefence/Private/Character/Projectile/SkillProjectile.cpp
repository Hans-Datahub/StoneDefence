// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Projectile/SkillProjectile.h"

// Sets default values
ASkillProjectile::ASkillProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 10.f;
}

// Called when the game starts or when spawned
void ASkillProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

