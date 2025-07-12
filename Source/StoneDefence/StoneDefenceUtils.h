// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Data/Core/CharacterData.h"
#include "Engine/StaticMeshActor.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Character/Projectile/SkillProjectile.h"
#include "Core/GameCore/TD_GameState.h"

#include "CHaracter/CharacterCore/Towers.h"
//#include "Engine/RawMesh.h"

class AActor;
class UStaticMesh;
class USkeletalMeshComponent;
class UWorld;
class AStaticMeshActor;
struct FRawMesh;


namespace StoneDefenceUtils {

	void CallUpdateAllClient(UWorld* World, TFunction<void(ATD_PlayerController* MyPlayerController)> InImplement);

	ARuleOfProjectile* SpawnProjectile(UWorld* World, ARuleOfCharacter* Owner, const int32 SKillID, const FVector& Loc, const FRotator& Rot);
	ARuleOfProjectile* SpawnProjectile(UWorld* World, FGuid CharacterID, UClass* ProjectileClass);//用于服务端通知客户端
	ARuleOfProjectile* SpawnProjectile(UWorld* World, APawn* NewPawn, UClass* InClass, const FVector& Loc, const FRotator& Rot);//用于动画通知
	//SpawnSkillProjectile = SpawnPlayerBullet
	ASkillProjectile* SpawnSkillProjectile(UWorld* World, int32 SkillID);

	AStaticMeshActor* SpawnTowerDoll(UWorld* World, int32 ID);

	void FindMostClosedTargetInRange(ARuleOfCharacter* InOwner, float Range, TArray<ARuleOfCharacter*> TargetArray);

	ARuleOfCharacter* FindMostClosedTarget(const TArray<ARuleOfCharacter*>& InCharacters,const FVector &Location);
	
	template<class T1, class T2>
	void GetAllActor(UWorld* World, TArray<T2*>& Array) {
		for (TActorIterator<T1>It(World, T1::StaticClass()); It; ++It) {
			if (T2* A = Cast<T2>(*It)) {//T1更改为T2后，*It到A的转换便由隐式转换变为了显式转换（即Cast），不知为何教程为T1
				Array.Add(A);
			}
		}
	}

	template<class T>
	void GetAllActor(UWorld *World, TArray<T*>& Array) {
		for (TActorIterator<T>It(World, T::StaticClass()); It; ++It)
			Array.Add(*It);
	}

	template<class T>
	TArray<T*> GetAllActor(UWorld* World) {
		TArray<T*> Array;
		for (TActorIterator<T>It(World); It; ++It) {//将 It的类型T::StaticClass()移除
			Array.Add(*It);
		}
		return Array;
	}

	ARuleOfCharacter* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator);
	template<class T>
	T* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator) {
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}

	//Execution() -> FindFitTargetAndExecution()
	void FindFitTargetAndExecution(UWorld* World, const FGuid& CharacterID, TFunction<void(ARuleOfCharacter* InCharacter)>Code);
}

namespace Expression {
	float GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Owner);
}

namespace MeshUtils {
	struct FMeshTracker;
	//@prama World:将mesh绑定到这个world上，在world销毁之后同时销毁mesh
	UStaticMesh* SkeletalMeshComponentToStaticMesh(class USkeletalMeshComponent* SkeletalMeshComponent);
	UStaticMesh* ParticleSystemComponentToStaticMesh(class UParticleSystemComponent* ParticleComponent);
	bool IsValidSkeletalMeshComponent(USkeletalMeshComponent* InComponent);
	void SkeletalMeshToRawMeshes(USkeletalMeshComponent* InComponent, int32 InOverallMaxLODs, const FMatrix& InComponentToWorld, FMeshTracker& MeshTracker, FRawMesh& RawMesh);
}

