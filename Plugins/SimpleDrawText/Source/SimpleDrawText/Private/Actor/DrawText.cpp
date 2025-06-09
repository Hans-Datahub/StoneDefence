// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DrawText.h"
#include "UI/UI_InformationWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADrawText::ADrawText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ADrawText::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloatClass.ExternalCurve) {
		FOnTimelineFloat TimeLineDelegate;
		FOnTimelineEvent FinishedEvent;
		TimeLineDelegate.BindUFunction(this, FName("TimeLineRangeTime"));
		FinishedEvent.BindUFunction(this, FName("TimeLineFinished"));
		TimeLine.AddInterpFloat(CurveFloatClass.ExternalCurve, TimeLineDelegate);//向时间线中添加浮点差值
		TimeLine.SetLooping(false);
		TimeLine.PlayFromStart();
		TimeLine.SetTimelineFinishedFunc(FinishedEvent);
	}
	
}

// Called every frame
void ADrawText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurveFloatClass.ExternalCurve) {
		TimeLine.TickTimeline(DeltaTime);
	}

}

void ADrawText::TimeLineRangeTime(float value) {
	//伤害字体漂浮效果
	if (Height) {
		FVector NewHeight = FMath::Lerp(FVector::ZeroVector, FVector(0, 0, Height), value);
		Widget->AddRelativeLocation(NewHeight);
	}
}

void ADrawText::TimeLineFinished() {
	Destroy(true);
}

void ADrawText::SetTextBlock(const FString& InText, const FLinearColor& Color, float Percentage) {
	if (UUI_InformationWidget* InfoWidget = Cast<UUI_InformationWidget>(Widget->GetUserWidgetObject())) {
		InfoWidget->SetTextBlockStyle(InText, Color, Percentage);
	}
}