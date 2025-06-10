// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StoneDefenceType.h"
#include "Components/SplineComponent.h"
#include "RuleOfProjectile.generated.h"

class ARuleOfCharacter;

UCLASS()
class STONEDEFENCE_API ARuleOfProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuleOfProjectile();

	//碰撞箱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class USphereComponent* ProjectileCollision;
	//作为根组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class USceneComponent* RootProjectile;
	//移动组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ChainAttack();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* ObjectiveOverlappedComponent,/*主体所重叠的组件*/
					  AActor* SubjectiveActor,						   /*目标Actor*/
					  UPrimitiveComponent* SubjectiveComp,			  /*目标重叠组件*/
					  int32 SubjectIndex,							 /*目标ID*/
					  bool IsScanning,								/*是否开始扫描*/
					  const FHitResult& ScanResult);			   /*扫描结果*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TEnumAsByte<EProjectileType> ProjectileType;
	//子弹伤害特效
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UParticleSystem* DamageParticle;
	//开火特效
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UParticleSystem* OpenFireParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		int32 SkillID;//所属技能ID

	UPROPERTY(EditDefaultsOnly, Category = "Projectile_Tracking2")
	float SplineOffset;
	UFUNCTION()
	void RadialDamage(const FVector& Origin, ARuleOfCharacter* InstigatorInOverlap);



	const FSkillData* GetSkillData(int32 SkillID);

	UFUNCTION(BlueprintCallable, Category = "SkillAdding Request")
	void SubmissionSkillRequest();


private:
	UPROPERTY()
	USplineComponent* Spline;

	UPROPERTY()
	float CurrentSplineTime;

	FTimerHandle ChainAttackHandle;

	UPROPERTY()
	uint8 ChainAttackCount;

};
