// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SpawnPoint.h"

ASpawnPoint::ASpawnPoint() 
	:Team(ETeam::BLUE)
{

}

void ASpawnPoint::BeginPlay(){
	Super::BeginPlay();
}