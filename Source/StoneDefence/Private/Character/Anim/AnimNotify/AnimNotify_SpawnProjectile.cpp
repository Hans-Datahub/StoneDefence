// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_SpawnProjectile.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Components/ArrowComponent.h"
#include "../StoneDefenceUtils.h"

class FCharacter;

UAnimNotify_SpawnProjectile::UAnimNotify_SpawnProjectile()
	:Super()
{
#if WITH_EDITORONLY_DATA
	InSocketName = TEXT("OpenFire");
	NotifyColor = FColor(196,142,255,255);
#endif 
}


FString UAnimNotify_SpawnProjectile::GetNotifyName_Implementation() const {
	if (ProjectileClass) {
		return ProjectileClass->GetName();
	}
	else {
		return Super::GetNotifyName_Implementation();
	}
	
}


void UAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(MeshComp->GetOuter());
#if WITH_EDITORONLY_DATA
	//当事件触发，传入骨骼名和动画名以获得Transform
	FVector ComponentLocation = MeshComp->GetSocketLocation(InSocketName);
	FRotator ComponentRotation = MeshComp->GetSocketRotation(InSocketName);
	
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
}
