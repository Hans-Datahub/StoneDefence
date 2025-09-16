// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings/UI_AudioSettings.h"
#include "Core/SimpleGameUserSettings.h"
#include "SimpleGameSettingsMacro.h"
#include "Components/Button.h"
#include "Components/AudioComponent.h"
#include "Sound/AmbientSound.h"

void UUI_AudioSettings::SaveSettings()
{
	SetSettingsLevel(AudioQualityLevelSlider, AudioQualityLevelText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetAudioQualityLevel(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsVlaue(BackgroundSoundSlider, BackgroundSoundText,
		[](float InValue)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetBackgroundSound(InValue);
		});

	SetSettingsVlaue(GameSoundSlider, GameSoundText,
		[](float InValue)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetGameSound(InValue);
		});

	SetSettingsVlaue(UISoundSlider, UISoundText,
		[](float InValue)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetUISound(InValue);
		});
}

void UUI_AudioSettings::LoadSettings()
{
	LoadSettingsLevel(AudioQualityLevelSlider, AudioQualityLevelText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAudioQualityLevel() / SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsVlaue(GameSoundSlider, GameSoundText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetGameSound();
		});

	LoadSettingsVlaue(UISoundSlider, UISoundText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetUISound();
		});

	LoadSettingsVlaue(BackgroundSoundSlider, BackgroundSoundText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetBackgroundSound();
		});
}

void UUI_AudioSettings::NativeConstruct()
{
	Super::NativeConstruct();

	BindChangedValue(AudioQualityLevelSlider);
	BindChangedValue(BackgroundSoundSlider);
	BindChangedValue(GameSoundSlider);
	BindChangedValue(UISoundSlider);

	GameSoundButton->OnClicked.AddDynamic(this, &UUI_AudioSettings::SpawnGameSoundForTest);
	UISoundButton->OnClicked.AddDynamic(this, &UUI_AudioSettings::SpawnUISoundForTest);
	BackgroundSoundButton->OnClicked.AddDynamic(this, &UUI_AudioSettings::SpawnBackgroundSoundForTest);
}

void UUI_AudioSettings::SpawnGameSoundForTest()
{
	DestroyBackgroundSound();

	if (GameSoundTestObject)
	{
		USimpleGameUserSettings::GetSimpleGameUserSettings()->SpawnSoundAtLocation(GameSoundTestObject, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	}
}

void UUI_AudioSettings::SpawnUISoundForTest()
{
	DestroyBackgroundSound();
}

void UUI_AudioSettings::SpawnBackgroundSoundForTest()
{
	if (BackgroundSoundTestObject)
	{
		DestroyBackgroundSound();

		FActorSpawnParameters Parameters;
		Parameters.Name = TEXT("BackgroundSoundTest");
		if (AAmbientSound* NewSound = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Parameters))
		{
			NewSound->GetAudioComponent()->SetSound(BackgroundSoundTestObject);
			NewSound->SetLifeSpan(BackgroundSoundTestObject->Duration);
			NewSound->Play();
		}
	}
}

void UUI_AudioSettings::DestroyBackgroundSound()
{
	TArray<AAmbientSound*> NewSounds;
	USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAllAmbientSound(NewSounds);
	for (AAmbientSound* Tmp : NewSounds)
	{
		if (Tmp->GetName() == TEXT("BackgroundSoundTest"))
		{
			Tmp->Stop();
			Tmp->Destroy();
			break;
		}
	}
}

void UUI_AudioSettings::ChangedValue(float InValue)
{
	UpdateAttibeLevel(AudioQualityLevelSlider, AudioQualityLevelText);	//Audio

	UpdateAttibe(UISoundSlider, UISoundText);
	UpdateAttibe(GameSoundSlider, GameSoundText);
	UpdateAttibe(BackgroundSoundSlider, BackgroundSoundText);

	SaveSettings();
}
