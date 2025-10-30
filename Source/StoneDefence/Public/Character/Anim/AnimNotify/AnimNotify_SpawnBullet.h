// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Character/Projectile/RuleOfProjectile.h"
#include "AnimNotify_SpawnBullet.generated.h"

class ARuleOfProjectile;
/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Spawn Projectile"))
class STONEDEFENCE_API UAnimNotify_SpawnBullet : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_SpawnBullet();

	virtual FString GetNotifyName_Implementation()const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
	FVector GetSimpleConeSpread(const FVector& BaseDirection, float SpreadAngle);
	void DrawConeSpreadDebug(USkeletalMeshComponent* MeshComp, const FVector& Origin, const FVector& Direction, float SpreadAngle, float Distance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	TSubclassOf<ARuleOfProjectile> ProjectileClass;


	//随机弹道参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	float BulletSpreadAngle = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	float DrawedSpreadDistance = 2200;

	FVector CachedTargetLocation;



#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FName InSocketName;
#endif
};
