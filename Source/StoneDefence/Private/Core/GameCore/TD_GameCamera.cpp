 // Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_GameCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATD_GameCamera::ATD_GameCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Set the default Tag
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("BOOM"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main_Camera"));
	CameraCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Sign"));

	//Attach Component to the RootComponent
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MainCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	CameraCollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Set the default Values
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	////设置相机的碰撞通道
	//CameraCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CameraCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//CameraCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	//CameraCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);


	//Set the default Transformer
	SetActorLocation(FVector(-1500.0f, 450.f, 1700.f));
}

// Called when the game starts or when spawned
void ATD_GameCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATD_GameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
//void ATD_GameCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

void ATD_GameCamera::Zoom(bool bDirection, const float ZoomSpeed) {
	if (bDirection) {
		if (CameraBoom->TargetArmLength > 0) {
			CameraBoom->TargetArmLength -= ZoomSpeed * 4;
		}
	}
	else {
		if (CameraBoom->TargetArmLength < 4000) {
			CameraBoom->TargetArmLength += ZoomSpeed * 4;


		}
	}
	//屏幕显示相机悬臂长度
	/*FString Message = FString::Printf(TEXT("TargetArmLength: %.2f"), CameraBoom->TargetArmLength);
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Blue, Message);*/
}
