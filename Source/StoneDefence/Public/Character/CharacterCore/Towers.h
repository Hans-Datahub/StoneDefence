// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Towers.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API ATowers : public ARuleOfCharacter
{
	GENERATED_BODY()

	//特效粒子
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* ParticleMesh;

	//建造预览模型，可建造为绿，不可建造为红
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* StaticMeshBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = true))
	class UDestructibleComponent* DestructibleMeshBuilding;


public:
	ATowers();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute")
	FRotator TowerRotation;

	virtual EGameCharacterType::Type GetCharacterType();
	virtual ETeam GetTeamType();
	virtual void RegisterTeam();

	UFUNCTION()
	virtual void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
