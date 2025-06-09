// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Data/Core/CharacterData.h"
#include "Engine/StaticMeshActor.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Core/GameCore/TD_GameState.h"


class AActor;
class UStaticMesh;
class USkeletalMeshComponent;
class UWorld;
class AStaticMeshActor;


namespace StoneDefenceUtils {

	AActor* SpawnProjectile(UWorld* World, FGuid CharacterID, UClass* ProjectileClass);//���ڷ����֪ͨ�ͻ���
	AActor* SpawnProjectile(UWorld* World, APawn* NewPawn, UClass* InClass, const FVector& Loc, const FRotator& Rot);//���ڶ���֪ͨ
	
	AStaticMeshActor* SpawnTowerDoll(UWorld* World, int32 ID);

	void FindMostClosedTargetInRange(ARuleOfCharacter* InOwner, float Range, TArray<ARuleOfCharacter*> TargetArray);

	ARuleOfCharacter* FindMostClosedTarget(const TArray<ARuleOfCharacter*>& InCharacters,const FVector &Location);
	
	template<class T1, class T2>
	void GetAllActor(UWorld* World, TArray<T2*>& Array) {
		for (TActorIterator<T1>It(World, T1::StaticClass()); It; ++It) {
			if (T2* A = Cast<T1>(*It)) {
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
		for (TActorIterator<T>It(World); It; ++It) {//�� It������T::StaticClass()�Ƴ�
			Array.Add(*It);
		}
		return Array;
	}

	ARuleOfCharacter* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator);
	template<class T>
	T* SpawnCharacter(int32 CharacterID, int32 CharacterLevel, UDataTable* InCharacterData, const FVector& Location, const FRotator& Rotator) {
		return Cast<T>(SpawnCharacter(CharacterID, CharacterLevel, InCharacterData, Location, Rotator));
	}
}

namespace Expression {
	float GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Owner);
}

namespace MeshUtils {
	struct FMeshTracker;
	//@prama World:��mesh�󶨵����world�ϣ���world����֮��ͬʱ����mesh
	UStaticMesh* SkeletalMeshComponentToStaticMesh(class USkeletalMeshComponent* SkeletalMeshComponent);
	UStaticMesh* ParticleSystemComponentToStaticMesh(class UParticleSystemComponent* ParticleComponent);
	bool IsValidSkeletalMeshComponent(USkeletalMeshComponent* InComponent);
	void SkeletalMeshToRawMeshes(USkeletalMeshComponent* InComponent, int32 InOverallMaxLODs, const FMatrix& InComponentToWorld, FMeshTracker& MeshTracker, FRawMesh& RawMesh);
}

