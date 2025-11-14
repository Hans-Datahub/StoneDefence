// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_SpawnBullet.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/CharacterCore/Militia.h"
#include "Character/Core/RuleOfAIController.h"
#include "Components/ArrowComponent.h"
#include "../StoneDefenceUtils.h"

#include "Kismet/KismetMathLibrary.h"

class FCharacter;

UAnimNotify_SpawnBullet::UAnimNotify_SpawnBullet()
	:Super()
{
#if WITH_EDITORONLY_DATA
	InSocketName = TEXT("OpenFireSocket");
	NotifyColor = FColor(196,142,255,255);
#endif 
}


FString UAnimNotify_SpawnBullet::GetNotifyName_Implementation() const {
	if (ProjectileClass) {
		return ProjectileClass->GetName();
	}
	else {
		return Super::GetNotifyName_Implementation();
	}
	
}


void UAnimNotify_SpawnBullet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	//获取武器Mesh，进而获取OpenFireSocket
	ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter());
	USkeletalMeshComponent* WeaponMesh = nullptr;
	FName WeaponSocketName = "WeaponSlot_Index"; // 你在蓝图中设置的Socket名称

	if (Character->GetMesh()->DoesSocketExist(WeaponSocketName))
	{
		// 获取附加到Socket的Actor
		TArray<AActor*> AttachedActors;
		Character->GetAttachedActors(AttachedActors);

		for (AActor* AttachedActor : AttachedActors) {
			if (USkeletalMeshComponent* FoundMesh = AttachedActor->FindComponentByClass<USkeletalMeshComponent>())
			{
				WeaponMesh = FoundMesh;
				break; // 找到后退出循环
			}
		}
	}
	
#if WITH_EDITORONLY_DATA

	FVector ComponentLocation = WeaponMesh->GetComponentLocation();

	//获取精准子弹Rotation
	TWeakObjectPtr<ARuleOfCharacter> Target = Character->GetController<ARuleOfAIController>()->Target;

	//缓存目标位置,以防目标消失，子弹找不到位置
	if (Target.IsValid())
		CachedTargetLocation = Character->GetController<ARuleOfAIController>()->Target->GetActorLocation();

	FVector TargetLocation = CachedTargetLocation;//目标死后，用最后的位置
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(InSocketName);
	FRotator ComponentRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetLocation);

	//获取随机子弹Rotation
	FVector RandomVector = GetSimpleConeSpread(TargetLocation - MuzzleLocation, BulletSpreadAngle);
	FRotator RandomRotation = RandomVector.Rotation();
#else
	FVector ComponentLocation = Character->GetFirePoint()->GetComponentLocation();
	FRotator ComponentRotation = Character->GetFirePoint()->GetComponentRotation();
#endif




	if (ARuleOfCharacter* AnimNotifyCharacter = Cast<ARuleOfCharacter>(MeshComp->GetOuter())){//此处不能用GetOwner()，而要用GetOuter() 四章三节17:30作更改
		if (ARuleOfProjectile* Projectile = StoneDefenceUtils::SpawnProjectile(Character->GetWorld(), AnimNotifyCharacter, ProjectileClass, ComponentLocation, ComponentRotation)) {
			//在生成普通攻击子弹时，将子弹的自动提交技能初始化枚举设置为MANUAL，这样就会跳过初始化技能，这就是一颗普通子弹
			//当生成技能效果子弹时，默认枚举为AUTO，则进行技能初始化
			Projectile->SubmissionSkillRequestType = ESubmissionSkillRequestType::MANUAL;
			Projectile->InitSkill();
		}
	}

	//调试
	// 绘制弹道线
	//DrawDebugLine(
	//	Character->GetWorld(),
	//	MuzzleLocation,
	//	TargetLocation,
	//	FColor::Red,        // 颜色
	//	false,              // 是否持续
	//	0.1f,           // 显示时间
	//	0,                  // 深度优先级
	//	3.0f                // 线宽
	//);

	//绘制锥形散布
	//DrawConeSpreadDebug(MeshComp, MuzzleLocation, (TargetLocation - MuzzleLocation).GetSafeNormal(), BulletSpreadAngle, DrawedSpreadDistance);

}

FVector UAnimNotify_SpawnBullet::GetSimpleConeSpread(const FVector& BaseDirection, float SpreadAngle)
{
	// 使用UE4的内置函数生成锥形随机方向
	return FMath::VRandCone(BaseDirection, FMath::DegreesToRadians(SpreadAngle));
}

void UAnimNotify_SpawnBullet::DrawConeSpreadDebug(USkeletalMeshComponent* MeshComp, const FVector& Origin, const FVector& Direction, float SpreadAngle, float Distance)
{
	ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter());
	if (!Character) return;
	UWorld* World = Character->GetWorld();
	if (!World) return;

	float SpreadRadians = FMath::DegreesToRadians(SpreadAngle);
	FVector RightVector = FVector::CrossProduct(Direction, FVector::UpVector);
	if (RightVector.IsNearlyZero())
	{
		RightVector = FVector::CrossProduct(Direction, FVector::ForwardVector);
	}
	RightVector.Normalize();

	FVector UpVector = FVector::CrossProduct(RightVector, Direction);
	UpVector.Normalize();

	// 绘制中心线
	DrawDebugLine(World, Origin, Origin + Direction * Distance, FColor::Red, false, 5.0f, 0, 1.0f);

	// 绘制锥形边界
	int32 Segments = 16;
	for (int32 i = 0; i < Segments; i++)
	{
		float Angle = 2.0f * PI * i / Segments;//角度由PI/8， 每次递增PI/8，直至 2PI
		float CosAngle = FMath::Cos(Angle);
		float SinAngle = FMath::Sin(Angle);

		FVector SpreadDir = Direction + (RightVector * CosAngle * SpreadRadians) + (UpVector * SinAngle * SpreadRadians);
		SpreadDir.Normalize();

		DrawDebugLine(World, Origin, Origin + SpreadDir * Distance, FColor::Green, false, 5.0f, 0, 1.0f);
	}

	// 绘制锥形末端的圆环
	for (int32 i = 0; i < Segments; i++)
	{
		float Angle1 = 2.0f * PI * i / Segments;
		float Angle2 = 2.0f * PI * (i + 1) / Segments;

		float CosAngle1 = FMath::Cos(Angle1);
		float SinAngle1 = FMath::Sin(Angle1);
		float CosAngle2 = FMath::Cos(Angle2);
		float SinAngle2 = FMath::Sin(Angle2);

		FVector Point1 = Origin + (Direction + (RightVector * CosAngle1 * SpreadRadians) + (UpVector * SinAngle1 * SpreadRadians)).GetSafeNormal() * Distance;
		FVector Point2 = Origin + (Direction + (RightVector * CosAngle2 * SpreadRadians) + (UpVector * SinAngle2 * SpreadRadians)).GetSafeNormal() * Distance;

		DrawDebugLine(World, Point1, Point2, FColor::Blue, false, 5.0f, 0, 1.0f);
	}
}
