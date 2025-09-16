// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/UI_VideoSettings.h"
#include "Core/SimpleGameUserSettings.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/SpinBox.h"
#include "SimpleGameSettingsMacro.h"

void UUI_VideoSettings::NativeConstruct()
{
	Super::NativeConstruct();

	FullScreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_VideoSettings::FullScreenCheckClickedBox);
	WindowScreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_VideoSettings::WindowScreenCheckClickedBox);
	SupportPhysXCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_VideoSettings::SupportPhysXCheckClickedBox);
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_VideoSettings::VSyncCheckClickedBox);
	HDRDisplayCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_VideoSettings::EnableHDRDisplayOutput);
	FrameRateLimitSpinBox->OnValueChanged.AddDynamic(this, &UUI_VideoSettings::SetFrameRateLimit);

	BindChangedValue(AntiAliasingSlider);
	BindChangedValue(ShadowQualitySlider);
	BindChangedValue(TextureQualitySlider);
	BindChangedValue(PostProcessingSlider);
	BindChangedValue(EffectsSlider);
	BindChangedValue(FoliageSlider);
	BindChangedValue(ViewDistanceSlider);
	BindChangedValue(ShadingSlider);

	BindSelectionChanged(ResolutionBoxString);
	BindSelectionChanged(LanguageString);

	//语言设置
	LanguageString->SetSelectedOption(USimpleGameUserSettings::GetSimpleGameUserSettings()->GetCurrentLanguageType());
}

void UUI_VideoSettings::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_VideoSettings::SaveSettings()
{
	SetSettingsLevel(TextureQualitySlider, TextureQualityText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetTextureQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(ShadowQualitySlider, ShadowQualityText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetShadowQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(AntiAliasingSlider, AntiAliasingText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetAntiAliasingQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(PostProcessingSlider, PostProcessingText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetPostProcessingQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(EffectsSlider, EffectsText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetVisualEffectQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(FoliageSlider, FoliageText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFoliageQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(ViewDistanceSlider, ViewDistanceText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetViewDistanceQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});

	SetSettingsLevel(ShadingSlider, ShadingText,
		[](float InLevel)
		{
			USimpleGameUserSettings::GetSimpleGameUserSettings()->SetShadingQuality(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
		});
}

void UUI_VideoSettings::LoadSettings()
{
	LoadSettingsLevel(AntiAliasingSlider, AntiAliasingText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAntiAliasingQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(TextureQualitySlider, TextureQualityText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetTextureQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(ShadowQualitySlider, ShadowQualityText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetShadowQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(PostProcessingSlider, PostProcessingText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetPostProcessingQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(EffectsSlider, EffectsText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetVisualEffectQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(ViewDistanceSlider, ViewDistanceText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetViewDistanceQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(FoliageSlider, FoliageText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetFoliageQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadSettingsLevel(ShadingSlider, ShadingText,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetShadingQuality() / (float)SCAL_ABILITY_QUALITY_LEVEL;
		});

	LoadBoxString(ResolutionBoxString,
		[]()
		{
			FIntPoint ResolutionPoint = USimpleGameUserSettings::GetSimpleGameUserSettings()->GetScreenResolution();
			return FString::Printf(TEXT("%ix%i"), ResolutionPoint.X, ResolutionPoint.Y);
		});

	LoadBoxString(LanguageString,
		[]()
		{
			return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetCurrentLanguageType();
		});

	EWindowMode::Type InModeType = USimpleGameUserSettings::GetSimpleGameUserSettings()->GetFullscreenMode();
	switch (InModeType)
	{
	case EWindowMode::WindowedFullscreen:
	case EWindowMode::Fullscreen:
		FullScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
		WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		break;
	case EWindowMode::Windowed:
		FullScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
		break;
	case EWindowMode::NumWindowModes:
		break;
	}

	ECheckBoxState VSyncBoxState = USimpleGameUserSettings::GetSimpleGameUserSettings()->IsVSyncEnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	VSyncCheckBox->SetCheckedState(VSyncBoxState);

	ECheckBoxState HDRDisplayBoxState = USimpleGameUserSettings::GetSimpleGameUserSettings()->IsHDREnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	HDRDisplayCheckBox->SetCheckedState(HDRDisplayBoxState);

	DisplayNitsSpinBox->SetValue(USimpleGameUserSettings::GetSimpleGameUserSettings()->GetCurrentHDRDisplayNits());

	FrameRateLimitSpinBox->SetValue(USimpleGameUserSettings::GetSimpleGameUserSettings()->GetFrameRateLimit());
}

void UUI_VideoSettings::FullScreenCheckClickedBox(bool ClickedWidget)
{
	FullScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
	WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
}

void UUI_VideoSettings::WindowScreenCheckClickedBox(bool ClickedWidget)
{
	FullScreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	WindowScreenCheckBox->SetCheckedState(ECheckBoxState::Checked);
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
}

void UUI_VideoSettings::SupportPhysXCheckClickedBox(bool ClickedWidget)
{

}

void UUI_VideoSettings::VSyncCheckClickedBox(bool ClickedWidget)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetVSyncEnabled(ClickedWidget);
}

void UUI_VideoSettings::EnableHDRDisplayOutput(bool ClickedWidget)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->EnableHDRDisplayOutput(ClickedWidget, DisplayNitsSpinBox->GetValue());
}

void UUI_VideoSettings::SetFrameRateLimit(float NewValue)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetFrameRateLimit(NewValue);
}

void UUI_VideoSettings::ChangedValue(float InValue)
{
	UpdateAttibeLevel(AntiAliasingSlider, AntiAliasingText);	//锯齿
	UpdateAttibeLevel(ShadowQualitySlider, ShadowQualityText);//阴影
	UpdateAttibeLevel(TextureQualitySlider, TextureQualityText);//贴图
	UpdateAttibeLevel(PostProcessingSlider, PostProcessingText);//后期
	UpdateAttibeLevel(EffectsSlider, EffectsText);//特效
	UpdateAttibeLevel(FoliageSlider, FoliageText);//植被
	UpdateAttibeLevel(ViewDistanceSlider, ViewDistanceText);//视距
	UpdateAttibeLevel(ShadingSlider, ShadingText);//Shading
}

void UUI_VideoSettings::SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	//更新语言
	{
		FString LanguageTmp = LanguageString->GetSelectedOption();
		USimpleGameUserSettings::GetSimpleGameUserSettings()->SetCurrentLanguage(LanguageTmp);
	}

	//更新分辨率
	{
		FIntPoint ResolutionPoint;
		FString ResolutionTmp = ResolutionBoxString->GetSelectedOption();
		FString R, L;
		ResolutionTmp.Split(TEXT("x"), &R, &L);
		ResolutionPoint.X = FCString::Atoi(*R);
		ResolutionPoint.Y = FCString::Atoi(*L);
		USimpleGameUserSettings::GetSimpleGameUserSettings()->SetScreenResolution(ResolutionPoint);
	}
}
