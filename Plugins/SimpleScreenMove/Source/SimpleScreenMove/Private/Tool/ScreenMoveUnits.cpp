#include "Tool/ScreenMoveUnits.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

bool FScreenMoveUnits::ListenScreenMove(APlayerController *PlayerController, float &ScreenMoveSpeed /*= 100.f*/)
{
	return MoveDirection(PlayerController, CursorMove(PlayerController), ScreenMoveSpeed);
}

EScreenMoveState FScreenMoveUnits::CursorMove(const APlayerController *PlayerController)
{
	if (PlayerController)
	{
		//屏幕尺寸
		int32 SizeX = INDEX_NONE;
		int32 SizeY = INDEX_NONE;

		//鼠标位置
		float MousePostionX = INDEX_NONE;
		float MousePostionY = INDEX_NONE;

		//获取尺寸
		PlayerController->GetViewportSize(SizeX, SizeY);

		//获取鼠标的位置
		PlayerController->GetMousePosition(MousePostionX, MousePostionY);

		if (MousePostionX >= 0 && MousePostionX <= SizeX &&
			MousePostionY >= 0 && MousePostionY <= SizeY)
		{
			if (FMath::IsNearlyEqual(MousePostionX,0.0f,5.0f) && FMath::IsNearlyEqual(MousePostionY, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_LeftAndUP; 
			}
			else if (FMath::IsNearlyEqual(MousePostionX, SizeX, 5.0f) && FMath::IsNearlyEqual(MousePostionY, SizeY, 5.0f))
			{
				return EScreenMoveState::Screen_RightAndDown;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, SizeX, 5.0f) && FMath::IsNearlyEqual(MousePostionY, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_RightAndUP;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, 0.0f, 5.0f) && FMath::IsNearlyEqual(MousePostionY, SizeY, 5.0f))
			{
				return EScreenMoveState::Screen_LeftAndDown;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_Left;
			}
			else if (FMath::IsNearlyEqual(MousePostionY, 0.0f, 5.0f))
			{
				return EScreenMoveState::Screen_UP;
			}
			else if (FMath::IsNearlyEqual(MousePostionY, SizeY, 5.0f))
			{
				return EScreenMoveState::Screen_Down;
			}
			else if (FMath::IsNearlyEqual(MousePostionX, SizeX, 5.0f))
			{
				return EScreenMoveState::Screen_Right;
			}
		}
	}

	return EScreenMoveState::Screen_None;
}

bool FScreenMoveUnits::MoveDirection(APlayerController *PlayerController, EScreenMoveState ScreenMoveState, float &ScreenMoveSpeed  /*=100.f*/)
{
	FVector OffsetValue = FVector::ZeroVector;

	if (PlayerController && PlayerController->GetPawn())
	{
		// 直接获取相机Actor的方向
		AActor* CameraActor = PlayerController->GetPawn();

		// 使用相机Actor的世界方向
		FVector ForwardVector = CameraActor->GetActorForwardVector();
		FVector RightVector = CameraActor->GetActorRightVector();

		// 将Z轴设为0，确保水平移动
		ForwardVector.Z = 0;
		RightVector.Z = 0;
		ForwardVector.Normalize();
		RightVector.Normalize();

		switch (ScreenMoveState)
		{
		case Screen_UP:
			OffsetValue = ForwardVector * ScreenMoveSpeed;
			break;
		case Screen_Down:
			OffsetValue = -ForwardVector * ScreenMoveSpeed;
			break;
		case Screen_Right:
			OffsetValue = RightVector * ScreenMoveSpeed;
			break;
		case Screen_Left:
			OffsetValue = -RightVector * ScreenMoveSpeed;
			break;
		case Screen_RightAndUP:
			OffsetValue = (ForwardVector + RightVector).GetSafeNormal() * ScreenMoveSpeed;
			break;
		case Screen_RightAndDown:
			OffsetValue = (-ForwardVector + RightVector).GetSafeNormal() * ScreenMoveSpeed;
			break;
		case Screen_LeftAndUP:
			OffsetValue = (ForwardVector - RightVector).GetSafeNormal() * ScreenMoveSpeed;
			break;
		case Screen_LeftAndDown:
			OffsetValue = -(ForwardVector + RightVector).GetSafeNormal() * ScreenMoveSpeed;
			break;
		}

		PlayerController->GetPawn()->AddActorWorldOffset(OffsetValue);
	}
	return OffsetValue != FVector::ZeroVector;
}

