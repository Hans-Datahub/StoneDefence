// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HallCore/TD_HallPawn.h"

// Sets default values
ATD_HallPawn::ATD_HallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATD_HallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATD_HallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATD_HallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

