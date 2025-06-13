// Copyright Epic Games, Inc. All Rights Reserved.


#include "StoneDefenceUtils.h"
#include "StoneDefence.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/CharacterCore/Monsters.h"
#include "Character/CharacterCore/Towers.h"
#include "Modules/ModuleManager.h"

#include "Engine/StaticMesh.h"
#include "RawMesh/Public/RawMesh.h"

#include "SkeletalRenderPublic.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Components/SkeletalMeshComponent.h"

#include "Character/Projectile/RuleOfProjectile.h"
#include "Components/ArrowComponent.h"
#include "Core/GameCore/TD_PlayerController.h"


class ARuleOfCharacter;
class AStaticMeshActor;

struct MeshUtils::FMeshTracker {
	FMeshTracker()
		:bValidColors(false)
	{
		FMemory::Memset(bValidTexCoords, 0);//内存初始化
	}
	bool bValidTexCoords[MAX_MESH_TEXTURE_COORDS];
	bool bValidColors;
};

void StoneDefenceUtils::FindMostClosedTargetInRange(ARuleOfCharacter* InOwner, float Range, TArray<ARuleOfCharacter*> TargetArray) {
	if (InOwner && Range > 0.0f) {
		TArray<ARuleOfCharacter*> TempArray;
		StoneDefenceUtils::GetAllActor(InOwner->GetWorld(), TempArray);
		for (ARuleOfCharacter* Temp : TempArray)
			if (Temp->IsActive())
				if (InOwner->GetTeamType() != Temp->GetTeamType())
					if ((InOwner->GetActorLocation() - Temp->GetActorLocation()).Size() <= Range)
						TargetArray.Add(Temp);
	}

}


ARuleOfCharacter* StoneDefenceUtils::FindMostClosedTarget(const TArray<ARuleOfCharacter*>& InCharacters, const FVector& SubjectLocation) {
		if (InCharacters.Num()) {
		float TargetDistance = 9999999999;//用于迭代存储最小距离
		int32 Index = INDEX_NONE;//此处INDEX_NONE为-1，用于迭代存储最近的目标ID

		for (int32 i = 0; i < InCharacters.Num(); i++) {
			if (ARuleOfCharacter* ObjectCharacter = InCharacters[i]) {
				FVector ObjectLocation = ObjectCharacter->GetActorLocation();
				FVector DistanceVector = ObjectLocation - SubjectLocation;
				float Distance = DistanceVector.Size();

				if (Distance < TargetDistance && ObjectCharacter->IsActive()) {
					//迭代最近距离与其对象ID
					Index = i;
					TargetDistance = Distance;
				}
			}
		}
		if (Index != INDEX_NONE) {
			//返回最近塔的ID 
			return InCharacters[Index];
		}
	}
	//若没找到返回NULL
	return NULL;
}


AStaticMeshActor* StoneDefenceUtils::SpawnTowerDoll(UWorld* World, int32 ID) {
	AStaticMeshActor* OutActor = nullptr;

	if (World) {
		if (ATD_GameState* GameState = World->GetGameState<ATD_GameState>()) {
			const TArray<FCharacterData*>& InDatas = GameState->GetTowerDataFromTable();
			for (const auto& Temp : InDatas) {
				if (Temp->ID == ID) {
					UClass* NewClass = Temp->CharacterBlueprintKey.LoadSynchronous();//负载同步
					if (NewClass) {
						if (ARuleOfCharacter* RuleOfCharacter = World->SpawnActor<ARuleOfCharacter>(NewClass, FVector::ZeroVector, FRotator::ZeroRotator)) {
							//AStaticMeshActor
							if (class AStaticMeshActor* MeshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator)) {
								FTransform Transform;
								if (UStaticMesh* InMesh = RuleOfCharacter->GetDollMesh(Transform)) {
									MeshActor->GetStaticMeshComponent()->SetRelativeTransform(Transform);
									MeshActor->SetMobility(EComponentMobility::Movable);
									MeshActor->GetStaticMeshComponent()->SetStaticMesh(InMesh);
									MeshActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
									OutActor = MeshActor;
									RuleOfCharacter->Destroy();//只用静态网格体
								}
								else {
									MeshActor->Destroy();
									RuleOfCharacter->Destroy();
								}
							}
							else
								RuleOfCharacter->Destroy();
						}
					}
					break;
				}
			}
		}
	}
	return OutActor;
}

ARuleOfProjectile* StoneDefenceUtils::SpawnProjectile(UWorld* World, FGuid CharacterID, UClass* ProjectileClass) {
	TArray <ARuleOfCharacter*> Characters;
	StoneDefenceUtils::GetAllActor(World, Characters);

	for (auto& Temp : Characters) {
		if (Temp->GUID == CharacterID) {
			FTransform Transform;
			Transform.SetLocation(Temp->GetFirePoint()->GetComponentLocation());
			Transform.SetRotation(Temp->GetFirePoint()->GetComponentRotation().Quaternion());

			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.Instigator = Temp;

			if (ARuleOfProjectile* Projectile
				= World->SpawnActor<ARuleOfProjectile>
				(ProjectileClass, Transform, ActorSpawnParameters)) {
				return Projectile;
			} 	
		}
	}
	return nullptr;
}

ARuleOfProjectile* StoneDefenceUtils::SpawnProjectile(UWorld* World, APawn* NewPawn, UClass* InClass, const FVector& Loc, const FRotator& Rot) {
	if (World && NewPawn && InClass) {
		FTransform Transform;
		Transform.SetLocation(Loc);
		Transform.SetRotation(Rot.Quaternion());

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Instigator = NewPawn;

		if (ARuleOfProjectile* Projectile
			= World->SpawnActor<ARuleOfProjectile>
			(InClass, Transform, ActorSpawnParameters)) {
			return Projectile;
		}
	}
	return nullptr;
}

ARuleOfProjectile* StoneDefenceUtils::SpawnProjectile(UWorld* World, ARuleOfCharacter* Owner, const int32 SKillID, const FVector& Loc, const FRotator& Rot) {
	ARuleOfProjectile* NewProjectile = nullptr;
	if (World) {
		if (ATD_GameState* InGameState = World->GetGameState<ATD_GameState>()) {
			if (const FSkillData* InData = InGameState->GetSkillData(SKillID)) {
				if (ARuleOfProjectile* Projectile = StoneDefenceUtils::SpawnProjectile(World, Owner, InData->ProjectileClass, Loc, Rot)) {
					NewProjectile = Projectile;
				}
			}
		}
	}
	return NewProjectile;
}

void StoneDefenceUtils::FindFitTargetAndExecution(UWorld* World, const FGuid& CharacterID, TFunction<void(ARuleOfCharacter* InCharacter)>Code) {
	if (World) {
		TArray<ARuleOfCharacter*> CharacterArray;
		StoneDefenceUtils::GetAllActor(World, CharacterArray);
		for (ARuleOfCharacter* Temp : CharacterArray) {
			if (Temp->GUID == CharacterID) {
				Code(Temp);	//此处是把Temp传给了InCharacter，即传给Code这个函数的形参,并非反之	
				break;
			}
		}
	}
}

void StoneDefenceUtils::CallUpdateAllClient(UWorld* World, TFunction<void(ATD_PlayerController* MyPlayerController)> InImplement) {
	if (World) {
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It) {
			if (ATD_PlayerController* MyPlayerController = Cast<ATD_PlayerController>(It->Get())) {
				InImplement(MyPlayerController);
			}
		}
	}
}


/*----------------------------------------------------------------------------------------------------------------------*/


float Expression::GetDamage(IRuleCharacter* Enemy, IRuleCharacter* Taker) {
	if (Enemy && Taker) {
		return Enemy->GetCharacterData().GetAttack() / ((Taker->GetCharacterData().GetArmor() / 100.f) + 1);
	}
	return 0;
}


UStaticMesh* MeshUtils::SkeletalMeshComponentToStaticMesh(class USkeletalMeshComponent* SkeletalMeshComponent) {
	class UStaticMesh* StaticMesh = nullptr;

	if (UWorld* World = SkeletalMeshComponent->GetWorld()) {
		//原始Mesh数据，包含一些点的位置、颜色以及坐标等等
		FRawMesh RawMesh;//需要在Build.cs中包含模块
		FMeshTracker MeshTracker;
		int32 OverallMaxLODs = 0;

		const FTransform& InRootTransform = FTransform::Identity;//单位化以方便计算 
		FMatrix WorldToRoot = InRootTransform.ToMatrixWithScale().Inverse();
		FMatrix ComponentToWorld = SkeletalMeshComponent->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;

		if (IsValidSkeletalMeshComponent(SkeletalMeshComponent)) {
			SkeletalMeshToRawMeshes(SkeletalMeshComponent, OverallMaxLODs, ComponentToWorld, MeshTracker, RawMesh);
		}

		uint32 MaxInUseTextureCoordsinate = 0;//声明最大纹理坐标
		if (!MeshTracker.bValidColors) {
			RawMesh.WedgeColors.Empty();
		}

		for (uint32 TexCoordsIndex = 0; TexCoordsIndex < MAX_MESH_TEXTURE_COORDS; TexCoordsIndex++) {
			if (!MeshTracker.bValidTexCoords[TexCoordsIndex]) {
				RawMesh.WedgeTexCoords[TexCoordsIndex].Empty();
			}
			else {
				MaxInUseTextureCoordsinate = FMath::Max(MaxInUseTextureCoordsinate, TexCoordsIndex);
			}
		}

		if (RawMesh.IsValidOrFixable()) {
			FString MeshName = FGuid::NewGuid().ToString();
			StaticMesh = NewObject<UStaticMesh>(World, *MeshName, RF_Transient);//RF_Transient 保证该Mesh不会被保存，不会序列化到磁盘
			StaticMesh->InitResources();

			StaticMesh->SetLightingGuid(FGuid::NewGuid());

			//MaxInUseTextureCoordinate + 1指1号贴图位置为灯光贴图，(uint32)8 - 1指序列为7
			const uint32 LightMapIndex = FMath::Min(MaxInUseTextureCoordsinate + 1, (uint32)8 - 1);//获取灯光UV  

			FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
			SrcModel.BuildSettings.bRecomputeNormals = false;
			SrcModel.BuildSettings.bRecomputeTangents = false;
			SrcModel.BuildSettings.bRemoveDegenerates = true;
			SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
			SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
			SrcModel.BuildSettings.bGenerateLightmapUVs = true;
			SrcModel.BuildSettings.SrcLightmapIndex = 0;
			SrcModel.BuildSettings.DstLightmapIndex = LightMapIndex;
			SrcModel.SaveRawMesh(RawMesh);

			for (const UMaterialInterface* Material : SkeletalMeshComponent->GetMaterials()) {
				StaticMesh->GetStaticMaterials().Add(FStaticMaterial(const_cast<UMaterialInterface*>(Material)));
			}

			StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
			StaticMesh->LightMapCoordinateIndex = LightMapIndex;

			TArray<int32> UniqueMaterialIndices;
			for (int32 MaterialIndex : RawMesh.FaceMaterialIndices) {
				UniqueMaterialIndices.AddUnique(MaterialIndex);
			}

			for (int32 i = 0; i < UniqueMaterialIndices.Num(); i++) {
				StaticMesh->GetSectionInfoMap().Set(0, i, FMeshSectionInfo(UniqueMaterialIndices[i]));
			}
			StaticMesh->GetOriginalSectionInfoMap().CopyFrom(StaticMesh->GetSectionInfoMap());
			StaticMesh->Build(false);//false指不希望弹出对话框

		}

	}
	return StaticMesh;
}

UStaticMesh* MeshUtils::ParticleSystemComponentToStaticMesh(UParticleSystemComponent* ParticleComponent){
	UStaticMesh* NewStaticMesh = nullptr;
	if (ParticleComponent->Template && ParticleComponent->Template->Emitters.Num() > 0) {
		for (const UParticleEmitter* Temp : ParticleComponent->Template->Emitters) {
			if (Temp->LODLevels[0]->bEnabled) {
				if (UParticleModuleTypeDataMesh* MyParticleDataMesh =
					Cast<UParticleModuleTypeDataMesh>(Temp->LODLevels[0]->TypeDataModule)) {
					NewStaticMesh = Cast<UStaticMesh>(MyParticleDataMesh);
					break;
				}
			}
		}
	}
	return NewStaticMesh;
}

bool MeshUtils::IsValidSkeletalMeshComponent(USkeletalMeshComponent * InComponent) {
	return InComponent && InComponent->MeshObject && InComponent->IsVisible();
}



void MeshUtils::SkeletalMeshToRawMeshes(USkeletalMeshComponent* InComponent,
								int32 InOverallMaxLODs, 
								const FMatrix& InComponentToWorld, 
								FMeshTracker& MeshTracker,
								FRawMesh& RawMesh) {
	FSkeletalMeshLODInfo& SrcLODInfo = *(InComponent->SkeletalMesh->GetLODInfo(InOverallMaxLODs));
	
	TArray<FFinalSkinVertex> FinalVertices;
	InComponent->GetCPUSkinnedVertices(FinalVertices, InOverallMaxLODs);
	FSkeletalMeshRenderData& SkeletalMeshRenderData = InComponent->MeshObject->GetSkeletalMeshRenderData();
	FSkeletalMeshLODRenderData& LODData = SkeletalMeshRenderData.LODRenderData[InOverallMaxLODs];

	for (int32 VertIndex = 0; VertIndex < FinalVertices.Num(); ++VertIndex) {
		//教程方法
		//RawMesh.VertexPositions.Add((InComponentToWorld.TransformPosition(FinalVertices[VertIndex].Position));

		//UE5适配方法
		FVector4d TransformedPosition4d = InComponentToWorld.TransformPosition(FVector3d(FinalVertices[VertIndex].Position));
		FVector3d TransformedPosition3d = FVector3d(TransformedPosition4d.X, TransformedPosition4d.Y, TransformedPosition4d.Z);
		RawMesh.VertexPositions.Add(FVector3f(TransformedPosition3d));
	}

	const uint32 NumTexCoords = FMath::Min(LODData.StaticVertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords(), (uint32)MAX_MESH_TEXTURE_COORDS);
	const int32 NumSections = LODData.RenderSections.Num();

	const FRawStaticIndexBuffer16or32Interface& IndexBuffer = *LODData.MultiSizeIndexContainer.GetIndexBuffer();

	for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++) {//外层遍历Render Section (渲染段)
		const FSkelMeshRenderSection& SkelMeshSection = LODData.RenderSections[SectionIndex];
		if (InComponent->IsMaterialSectionShown(SkelMeshSection.MaterialIndex, InOverallMaxLODs)) {
			const int32 NumWedges = SkelMeshSection.NumTriangles * 3;
			for (int32 WedgeIndex = 0; WedgeIndex < NumWedges; WedgeIndex++) {//内层遍历处理三角锥数据
				//从缓冲区获取顶点索引
				const int32 VertexIndexForWedge = IndexBuffer.Get(SkelMeshSection.BaseIndex + WedgeIndex);
				RawMesh.WedgeIndices.Add(VertexIndexForWedge);

				//处理切线空间
				const FFinalSkinVertex& SkinnedVertex = FinalVertices[VertexIndexForWedge];
				const FVector TangentX = InComponentToWorld.TransformVector(SkinnedVertex.TangentX.ToFVector());
				const FVector TangentZ = InComponentToWorld.TransformVector(SkinnedVertex.TangentZ.ToFVector());
				const FVector4 UnpackedTangentZ = SkinnedVertex.TangentZ.ToFVector4();
				const FVector TangentY = (TangentZ ^ TangentX).GetSafeNormal() * UnpackedTangentZ.W;
				RawMesh.WedgeTangentX.Add(FVector3f(TangentX));
				RawMesh.WedgeTangentY.Add(FVector3f(TangentY));
				RawMesh.WedgeTangentZ.Add(FVector3f(TangentZ));

				//处理UV坐标
				for (uint32 TexCoordIndex = 0; TexCoordIndex < MAX_MESH_TEXTURE_COORDS; TexCoordIndex++) {
					if (TexCoordIndex >= NumTexCoords) {
						RawMesh.WedgeTexCoords[TexCoordIndex].AddDefaulted();
					}
					else {
						RawMesh.WedgeTexCoords[TexCoordIndex].Add(LODData.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIndexForWedge, TexCoordIndex));
						MeshTracker.bValidTexCoords[TexCoordIndex] = true;
					}
				}

				//处理顶点颜色
				if (LODData.StaticVertexBuffers.ColorVertexBuffer.IsInitialized()) {
					RawMesh.WedgeColors.Add(LODData.StaticVertexBuffers.ColorVertexBuffer.VertexColor(VertexIndexForWedge));
					MeshTracker.bValidColors = true;
				}
				else {
					RawMesh.WedgeColors.Add(FColor::White);
				}
			}

			//处理材质索引
			int32 MaterialIndex = SkelMeshSection.MaterialIndex;
			if (SrcLODInfo.LODMaterialMap.IsValidIndex(SectionIndex) && SrcLODInfo.LODMaterialMap[SectionIndex] != INDEX_NONE) {
				MaterialIndex = FMath::Clamp<int32>(SrcLODInfo.LODMaterialMap[SectionIndex], 0, InComponent->SkeletalMesh->Materials.Num());
			}
			for (uint32 TriIndex = 0; TriIndex < SkelMeshSection.NumTriangles; TriIndex++) {
				RawMesh.FaceMaterialIndices.Add(MaterialIndex);
				RawMesh.FaceSmoothingMasks.Add(0);
			}
		}
	}
}


