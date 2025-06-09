// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StoneDefenceType.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Components/SplineComponent.h"
#include "RuleOfProjectile.generated.h"

UCLASS()
class STONEDEFENCE_API ARuleOfProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuleOfProjectile();

	//��ײ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class USphereComponent* ProjectileCollision;
	//��Ϊ�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class USceneComponent* RootProjectile;
	//�ƶ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ChainAttack();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* ObjectiveOverlappedComponent,/*�������ص������*/
					  AActor* SubjectiveActor,						   /*Ŀ��Actor*/
					  UPrimitiveComponent* SubjectiveComp,			  /*Ŀ���ص����*/
					  int32 SubjectIndex,							 /*Ŀ��ID*/
					  bool IsScanning,								/*�Ƿ�ʼɨ��*/
					  const FHitResult& ScanResult);			   /*ɨ����*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TEnumAsByte<EProjectileType> ProjectileType;
	//�ӵ��˺���Ч
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UParticleSystem* DamageParticle;
	//������Ч
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UParticleSystem* OpenFireParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile_Tracking2")
	float SplineOffset;
	UFUNCTION()
	void RadialDamage(const FVector& Origin, ARuleOfCharacter* InstigatorInOverlap);



private:
	UPROPERTY()
	USplineComponent* Spline;

	UPROPERTY()
	float CurrentSplineTime;

	FTimerHandle ChainAttackHandle;

	UPROPERTY()
	uint8 ChainAttackCount;

};
