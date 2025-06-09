// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/TD_PlayerController.h"
#include "Core/GameCore/TD_GameCamera.h"
#include "Components/InputComponent.h"


ATD_PlayerController::ATD_PlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ATD_PlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	float ScreenMoveSpeed = 20.f;
	ScreenMoveUnits.ListenScreenMove(this, ScreenMoveSpeed);


	if (TowerDoll) {
		if (MouseTraceHit.Location != FVector::ZeroVector)
			MouseTraceHit = FHitResult();
		FHitResult TraceOutHit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel4, true, TraceOutHit);
		TowerDoll->SetActorLocation(TraceOutHit.Location);
	}
	else {
		//�����Ļ�е����͹��ﲢ����Ļ������Tip
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel5, true, MouseTraceHit);
	}
}

void ATD_PlayerController::BeginPlay() {
	Super::BeginPlay();

	//�������
	//SetInputMode_GameAndUIEx����
	//�������ͼ������SetInputModeGameAndUI()
	SetInputModeGameAndUI();
}  


ATowers* ATD_PlayerController::SpawnTower(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator) {
	if (GetGameMode()) {
		return GetGameMode()->SpawnTower(CharacterID, CharacterLevel, Location, Rotator);
	}
	return nullptr;
}


AMonsters* ATD_PlayerController::SpawnMonster(int32 CharacterID, int32 CharacterLevel, const FVector& Location, const FRotator& Rotator) {
	if (GetGameMode()) {
		return GetGameMode()->SpawnMonster(CharacterID, CharacterLevel, Location, Rotator);
	}
	return nullptr;
}


ATD_GameMode* ATD_PlayerController::GetGameMode() {
	return GetWorld()->GetAuthGameMode<ATD_GameMode>();
}

void ATD_PlayerController::SetInputModeGameAndUI() {
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
}

void ATD_PlayerController::SetupInputComponent() {
	//ִ��һ�¸���
	Super::SetupInputComponent();
	//�����ְ� , ��һ������Ϊ�༭���еİ������ƣ��ڶ���Ϊ���»���̧�𣬵�����Ϊʵ�������ĸ�Ϊ���󶨺���
	InputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &ATD_PlayerController::MouseWheelUp);
	InputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &ATD_PlayerController::MouseWheelDown);
	InputComponent->BindAction("MouseMiddleButton", IE_Pressed, this, &ATD_PlayerController::MouseMiddleButtonPressed);
	InputComponent->BindAction("MouseMiddleButton", IE_Released, this, &ATD_PlayerController::MouseMiddleButtonReleased);
}

static float WheelValue = 15.f;
void ATD_PlayerController::MouseWheelUp() {
	ATD_GameCamera* ZoomCamera = Cast<ATD_GameCamera>(GetPawn());
	if (ZoomCamera) {
		ZoomCamera->Zoom(true, WheelValue);
	}
}

void ATD_PlayerController::MouseWheelDown() {
	ATD_GameCamera* ZoomCamera = Cast<ATD_GameCamera>(GetPawn());
	if (ZoomCamera) {
		ZoomCamera->Zoom(false,WheelValue);
	}
}


void ATD_PlayerController::MouseMiddleButtonPressed() {
	EventMouseMiddlePressed.ExecuteIfBound();
}
void ATD_PlayerController::MouseMiddleButtonReleased() {
	EventMouseMiddleReleased.ExecuteIfBound();
}


const FHitResult& ATD_PlayerController::GetHitResult() {
	return MouseTraceHit;
}

void ATD_PlayerController::AddSkillSlot_Client(const FGuid& SlotID) {
	SkillAddingDelegate.ExecuteIfBound(SlotID);
}

void ATD_PlayerController::Spawn_Projectile_Client(const FGuid& CharacterID, UClass* InClass) {
	ProjectileSpawnDelegate.ExecuteIfBound(CharacterID, InClass);
}