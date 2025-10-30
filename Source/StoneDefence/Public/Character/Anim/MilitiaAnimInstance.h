// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Anim/RuleOfAnimInstance.h"
#include "MilitiaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UMilitiaAnimInstance : public URuleOfAnimInstance
{
	GENERATED_BODY()

	UMilitiaAnimInstance();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float Horizontal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float Vertical;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float MoveForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimAttrubute")
	bool HasMoveOrder;

	void NativeUpdateAnimation(float DeltaSeconds);

	
};
