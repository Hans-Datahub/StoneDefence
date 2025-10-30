// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Projectile/WeaponBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AWeaponBullet::AWeaponBullet()
	:BulletSpeed(50000.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision")); //BoxDamage -> ProjectileCollision
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));

	RootComponent = RootProjectile;
	ProjectileCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement->MaxSpeed = BulletSpeed;
	ProjectileMovement->InitialSpeed = BulletSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	//ProjectileMovement->UpdatedComponent = RootComponent;

	ProjectileType = EProjectileType::PROJECTILE_BULLET;

	InitialLifeSpan = 1.0f;//声明周期为1秒 


	SkillID = INDEX_NONE;
	SubmissionSkillRequestType = ESubmissionSkillRequestType::AUTO;
	SkillTargetType = ESkillTargetType::ENEMY;
	//SkillID = 1;
}

