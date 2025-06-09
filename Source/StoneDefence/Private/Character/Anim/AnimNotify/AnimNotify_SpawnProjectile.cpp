// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/AnimNotify/AnimNotify_SpawnProjectile.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Components/ArrowComponent.h"
#include "../StoneDefenceUtils.h"

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

#if WITH_EDITORONLY_DATA
	//���¼�����������������Ͷ������Ի��Transform
	FVector ComponentLocation = MeshComp->GetSocketLocation(InSocketName);
	FRotator ComponentRotation = MeshComp->GetSocketRotation(InSocketName);
	if (APawn* Character = Cast<APawn>(MeshComp->GetOuter()))
#else
	FVector ComponentLocation = Character->GetFirePoint()->GetComponentLocation();
	FRotator ComponentRotation = Character->GetFirePoint()->GetComponentRotation();
#endif
	if (ARuleOfCharacter* AnimNotifyCharacter = Cast<ARuleOfCharacter>(MeshComp->GetOuter())){//�˴�������GetOwner()����Ҫ��GetOuter() ��������17:30������

		StoneDefenceUtils::SpawnProjectile(Character->GetWorld(), Cast<APawn>(AnimNotifyCharacter), ProjectileClass, ComponentLocation, ComponentRotation);

//		FTransform Transform;
//		Transform.SetLocation(ComponentLocation);
//		Transform.SetRotation(ComponentRotation.Quaternion());
//
//		FActorSpawnParameters ActorSpawnParameters;
////#if WITH_EDITORONLY_DATA  �̳�11-7 19:02�����˴�ɾ�� 
//		ActorSpawnParameters.Instigator = Character;
////#endif
//		if (ARuleOfProjectile* Projectile
//			= Character->GetWorld()->SpawnActor<ARuleOfProjectile>
//			(ProjectileClass, Transform, ActorSpawnParameters))
//		{}
	}
}
