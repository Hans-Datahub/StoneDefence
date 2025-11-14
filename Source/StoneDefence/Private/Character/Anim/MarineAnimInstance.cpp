// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/MarineAnimInstance.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/CharacterCore/Marine.h"
#include "Core/GameCore/TD_GameState.h"


UMarineAnimInstance::UMarineAnimInstance()
	:Horizontal(0.0f), Vertical(0.0f), MoveForward(0.0f), MoveRight(0.0f), HasMoveOrder(false)
{

}

void UMarineAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMarine* Marine = Cast<AMarine>(TryGetPawnOwner());
	if (Marine) {
		//设置动画播放与暂停//

		if (ATD_GameState* TempGameState = GetWorld()->GetGameState<ATD_GameState>()) {
			USkeletalMeshComponent* SkeletalMesh = Marine->GetMesh();
			if (TempGameState->GetGameData().bTimeFreezed == false)
				SkeletalMesh->SetPlayRate(1.0f);
			else
				SkeletalMesh->SetPlayRate(0.0f);
		}



		//更新AimOffset的基本参数//

		if (USkeletalMeshComponent* CharacterMesh = Marine->GetMesh())
		{
			// 获取控制器的旋转，如果没有控制器则使用角色自身的旋转
			FRotator ControlRotation = Marine->GetControlRotation();
			if (AController* Controller = Marine->GetController())
			{
				ControlRotation = Controller->GetControlRotation();
			}

			// 计算AimOffset的Yaw和Pitch
			FRotator AimRotation = ControlRotation;
			FRotator ActorRotation = Marine->GetActorRotation();

			// 将世界空间的旋转转换为角色局部空间的旋转
			FRotator LocalAimRotation = (AimRotation - ActorRotation).GetNormalized();

			// 设置AimOffset参数
			Horizontal = FMath::Clamp(LocalAimRotation.Yaw, -90.0f, 90.0f);
			Vertical = FMath::Clamp(LocalAimRotation.Pitch, -90.0f, 90.0f);
		}



		//更新移动参数//

		if (UCharacterMovementComponent* MovementComponent = Marine->GetCharacterMovement())
		{
			// 获取角色速度并转换为局部空间
			FVector Velocity = Marine->GetVelocity();
			FRotator Rotation = Marine->GetActorRotation();

			// 将世界空间速度转换为角色局部空间速度
			FVector LocalVelocity = Rotation.UnrotateVector(Velocity);

			// 设置前后移动和左右移动参数
			MoveForward = FMath::Clamp(LocalVelocity.X, -1.0f, 1.0f);
			MoveRight = FMath::Clamp(-LocalVelocity.Y, -1.0f, 1.0f);

			// 如果需要速度大小而不是标准化值，可以取消注释下面这行
			// MoveSpeed = Velocity.Size();
		}
	}



}