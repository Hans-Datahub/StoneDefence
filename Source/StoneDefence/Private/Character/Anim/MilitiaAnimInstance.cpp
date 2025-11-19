// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Anim/MilitiaAnimInstance.h"
#include "Character/CharacterCore/Militia.h"
#include "Core/GameCore/TD_GameState.h"
//#include "GameFramework/CharacterMovementComponent.h"

UMilitiaAnimInstance::UMilitiaAnimInstance()
	:Horizontal(0.0f), Vertical(0.0f), MoveForward(0.0f), MoveRight(0.0f), HasMoveOrder(false), HasMoveOrderFromPlayer(false)
{

}

void UMilitiaAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	//单独更新Militia的移动命令，Marine的由UBTService_HandleLocationToMoveFromPlayer来更新
	AMilitia* Militia = Cast<AMilitia>(TryGetPawnOwner());
	if (Militia) {
		//更新移动命令
		const float SpeedThreshold = 10.0f; // 速度阈值

		if (Militia->GetVelocity().Size() >= SpeedThreshold)
			HasMoveOrder = true;
		else
			HasMoveOrder = false;		
	}

	//余下的部分同时设置两者
	ARuleOfCharacter* Character = Cast<ARuleOfCharacter>(TryGetPawnOwner());
	if (!Character) { return; }

	//设置动画播放与暂停//
	if (ATD_GameState* TempGameState = GetWorld()->GetGameState<ATD_GameState>()) {
		USkeletalMeshComponent* SkeletalMesh = Character->GetMesh();
		if (TempGameState->GetGameData().bTimeFreezed == false)
			SkeletalMesh->SetPlayRate(1.0f);
		else
			SkeletalMesh->SetPlayRate(0.0f);
	}



	//更新基本参数供AimOffset使用//

	if (USkeletalMeshComponent* CharacterMesh = Character->GetMesh())
	{
		// 获取控制器的旋转，如果没有控制器则使用角色自身的旋转
		FRotator ControlRotation = Character->GetControlRotation();
		if (AController* Controller = Character->GetController())
		{
			ControlRotation = Controller->GetControlRotation();
		}

		// 计算AimOffset的Yaw和Pitch
		FRotator AimRotation = ControlRotation;
		FRotator ActorRotation = Character->GetActorRotation();

		// 将世界空间的旋转转换为角色局部空间的旋转
		FRotator LocalAimRotation = (AimRotation - ActorRotation).GetNormalized();

		// 设置AimOffset参数
		Horizontal = FMath::Clamp(LocalAimRotation.Yaw, -90.0f, 90.0f);
		Vertical = FMath::Clamp(LocalAimRotation.Pitch, -90.0f, 90.0f);
	}



	//更新移动参数（MoveForward和MoveRight）供混合空间使用//

	if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
	{
		// 获取角色速度并转换为局部空间
		FVector Velocity = Character->GetVelocity();
		FRotator Rotation = Character->GetActorRotation();

		// 将世界空间速度转换为角色局部空间速度
		FVector LocalVelocity = Rotation.UnrotateVector(Velocity);

		// 设置前后移动和左右移动参数
		MoveForward = FMath::Clamp(LocalVelocity.X, -1.0f, 1.0f);
		MoveRight = FMath::Clamp(-LocalVelocity.Y, -1.0f, 1.0f);

		// 如果需要速度大小而不是标准化值，可以取消注释下面这行
		// MoveSpeed = Velocity.Size();
	}

}