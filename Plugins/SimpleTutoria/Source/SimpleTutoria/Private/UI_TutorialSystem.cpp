// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TutorialSystem.h"

#include "Tutoria/UI_TutorialSlot.h"
#include "MediaAssets/Public/MediaPlayer.h"

#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

#include "HAL/FileManager.h"
#include "GlobalTutorialProxy.h"
#include "MediaPlayList.h"




void UUI_TutorialSystem::NativeConstruct() {
	Super::NativeConstruct();
	InitMedia();

	SimpleTutoriaMulticastDelegate.BindUObject(this, &UUI_TutorialSystem::Play);

	if (MediaPlayer)
	{
		MediaPlayer->OnEndReached.AddDynamic(this, &UUI_TutorialSystem::FinishPlayMovie);
	}

	ReplayButton->OnClicked.AddDynamic(this, &UUI_TutorialSystem::Replay);
	PauseButton->OnClicked.AddDynamic(this, &UUI_TutorialSystem::Pause);

	MovieProgress->OnMouseCaptureBegin.AddDynamic(this, &UUI_TutorialSystem::MouseCaptureBegin);
	MovieProgress->OnMouseCaptureEnd.AddDynamic(this, &UUI_TutorialSystem::MouseCaptureEnd);

	SuspendButton->OnCheckStateChanged.AddDynamic(this, &UUI_TutorialSystem::ClickedCheckBox);

	CloseButton->OnClicked.AddDynamic(this, &UUI_TutorialSystem::CloseUI);
}

void UUI_TutorialSystem::NativeTick(const FGeometry& MeGeometry, float InDeltaTime) {
	Super::NativeTick(MeGeometry, InDeltaTime);

	if (MediaPlayer) {
		float NewValue = MediaPlayer->GetTime().GetTotalSeconds() / MediaPlayer->GetDuration().GetTotalSeconds();
		MovieProgress->SetValue(NewValue);
		PlayTimeText->SetText(FText::FromString(MediaPlayer->GetTime().ToString() + "/" + MediaPlayer->GetDuration().ToString()));
	}

}

void UUI_TutorialSystem::InitMedia() { 
	if (MediaPlayer) {
		TArray<FString>MediaFileNames;
		FString MediaPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Media/"));
		IFileManager::Get().FindFilesRecursive(MediaFileNames, *MediaPath, TEXT("*"), true, false);
		for (int32 i = 0; i < MediaFileNames.Num(); i++) {
			if (UUI_TutorialSlot* TutorialSlot = CreateWidget<UUI_TutorialSlot>(GetWorld(), TutorialSlotClass)) {
				TutorialSlot->Index = i;
				MediaPlayer->GetPlaylist()->AddFile(MediaPath);
				ScrollMediaList->AddChild(TutorialSlot);
			}
		}
		if (UMediaSource* VedioGoingToPlay = MediaPlayer->GetPlaylist()->Get(0)) {
			MediaPlayer->OpenSource(VedioGoingToPlay);
		}
	}
}

void UUI_TutorialSystem::ActivateMovie() {
	if (MediaPlayer->IsPlaying()) {
		MediaPlayer->Pause();
		PauseImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SuspendButton->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else {
		MediaPlayer->Play();
		PauseImage->SetVisibility(ESlateVisibility::Hidden);
		SuspendButton->SetCheckedState(ECheckBoxState::Checked);
	}
}


bool UUI_TutorialSystem::Play(int32 Index) {
	if (UMediaSource* Media = MediaPlayer->GetPlaylist()->Get(Index)) {
		return MediaPlayer->OpenSource(Media);
	}
	return false;
}

void UUI_TutorialSystem::Replay() {
	if (MediaPlayer->IsReady()) {
		MediaPlayer->Rewind();
	}
}

void UUI_TutorialSystem::Close() {
	if (MediaPlayer->IsPlaying()) {
		MediaPlayer->Pause();
		PauseImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SuspendButton->SetCheckedState(ECheckBoxState::Unchecked);
	}
	MediaPlayer->Close();
}

void UUI_TutorialSystem::Pause() {
	ActivateMovie();
}

void UUI_TutorialSystem::FinishPlayMovie() {
	if (MediaPlayer->IsReady()) {
		MediaPlayer->Close();
	}
}

void UUI_TutorialSystem::ClickedCheckBox(bool ClickedWidget) {
	ActivateMovie();
}



void UUI_TutorialSystem::MouseCaptureBegin() {

}

void UUI_TutorialSystem::MouseCaptureEnd() {

}

void UUI_TutorialSystem::CloseUI() {
	RemoveFromParent();
}