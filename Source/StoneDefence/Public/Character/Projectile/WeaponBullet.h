// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Projectile/RuleOfProjectile.h"
#include "WeaponBullet.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API AWeaponBullet : public ARuleOfProjectile
{
	GENERATED_BODY()

	AWeaponBullet();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float BulletSpeed;
};
