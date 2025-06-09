// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../GlobalTutorialProxy.h"
#include "UI_TutorialSlot.generated.h"

class UMediaSource;
class UButton;

UCLASS()
class SIMPLETUTORIA_API UUI_TutorialSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	////载入视频源
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	//UMediaSource* MediaSource;
	//8-8  00:40处删除

	UUI_TutorialSlot(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	int32 Index;

	bool IsIndexValid();


	UPROPERTY(meta = (BindWidget))
		UButton* PlayButton;
	
	UFUNCTION()
		void Play();

protected:

	virtual void NativeConstruct()override;

};
