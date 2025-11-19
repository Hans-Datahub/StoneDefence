// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Data/CharacterData.h"
#include "Engine/StaticMeshActor.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Character/Projectile/SkillProjectile.h"
#include "Core/GameCore/TD_GameState.h"
#include "Components/SizeBox.h"
#include "CHaracter/CharacterCore/Towers.h"
//#include "Engine/RawMesh.h"

class AActor;
class UStaticMesh;
class USkeletalMeshComponent;
class UWorld;
class AStaticMeshActor;
struct FRawMesh;

class USizeBox;
class ARuleOfTheCharacter;
class IRuleCharacter;
class ARuleOfTheProjectile;
class APlayerSkillSlotActor;
class ATDPlayerController;
class ASceneCapture2D;


namespace StoneDefenceUtils {

	void CallUpdateAllClient(UWorld* World, TFunction<void(ATD_PlayerController* MyPlayerController)> InImplement);
	void CallUpdateAllBaseClient(UWorld* World, TFunction<void(APlayerController* MyPlayerController)> InImplement);

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


	template<class T>
	T* GetSave(UWorld* InWorld, const TCHAR* SaveName, int32 SaveIndex = INDEX_NONE, EGameSaveType InFlag = EGameSaveType::NONE)
	{
		T* InSlot = nullptr;

		auto InitSlot = [&]()
		{
			InSlot = Cast<T>(UGameplayStatics::CreateSaveGameObject(T::StaticClass()));
			if (InSlot)
			{
				InSlot->InitSaveGame(InWorld);
			}
		};

		if (InFlag & EGameSaveType::ARCHIVES)
		{
			FString SlotString;
			//字段处理
			if (FString(SaveName) == TEXT("SlotList")) {//若为SlotList，跳过字段修正
				SlotString = SaveName;
			}
			else if (FString(SaveName).Contains(TEXT("%i"))) {//若为PlayerData或者SaveSlot,修正后缀
				if (SaveIndex == INDEX_NONE)//若为自动存档
				{			
					SlotString = SlotString.Replace(TEXT("%i"), TEXT("0"));
				}
				else if(SaveIndex != INDEX_NONE)//若为手动存档
				{
					FString BaseName = SaveName;
					SlotString = BaseName.Replace(TEXT("%i"), *FString::FromInt(SaveIndex));
				}
			}


			//存档数据处理
			InSlot = Cast<T>(UGameplayStatics::LoadGameFromSlot(SlotString, 0));
			if (!InSlot)//若无存档，新建并初始化
			{
				InitSlot();
			}
			else//若有存档，则读取
			{
				InSlot->InitSaveGameFromArchives(InWorld);//无实现
			}
		}
		else
		{
			InitSlot();
		}

		return InSlot;
	}

	//面板Widget 切换
	template<class T, class UserObject>
	UserObject* CreateAssistWidget(T* ThisClass, UClass* AssistClass, USizeBox* WidgetArray)
	{
		UserObject* UserObjectElement = nullptr;
		//播放动画的判断
		if (0)
		{
			//播放 淡入
		}

		if (WidgetArray->GetChildAt(0))
		{
			if (WidgetArray->GetChildAt(0)->IsA(AssistClass))
			{
				//关闭我们的board 淡出

				return UserObjectElement;
			}
			else
			{
				WidgetArray->ClearChildren();
			}
		}

		UserObjectElement = CreateWidget<UserObject>(ThisClass->GetWorld(), AssistClass);
		if (UserObjectElement)
		{
			if (WidgetArray->AddChild(UserObjectElement))
			{
				//
			}
			else
			{
				UserObjectElement->RemoveFromParent();
			}
		}

		return UserObjectElement;
	}
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

namespace RenderingUtils
{
	struct FScreenShot
	{
		FScreenShot(
			int32 InWidth,
			int32 InHeight,
			UTexture2D*& InTexture,
			UObject* InOuter,
			int32 InImageQuality = 80,
			bool bInShowUI = false,
			bool bAddFilenameSuffix = true);

		FString& GetFilename() { return Filename; }
	protected:
		void OnScreenshotCapturedInternal(int32 SrcWidth, int32 SrcHeight, const TArray<FColor>& OrigBitmap);
	private:
		UTexture2D*& Texture;
		FDelegateHandle ScreenShotDelegateHandle;
		int32 ScaledWidth;
		int32 ScaledHeight;
		int32 ImageQuality;
		UObject* Outer;
		FString Filename;
	};

	ASceneCapture2D* SpawnSceneCapture2D(UWorld* World, UClass* SceneCaptureClass, FMapSize& MapSize, float Life = 0.f);
}