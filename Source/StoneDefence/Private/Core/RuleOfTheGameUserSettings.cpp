// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RuleOfTheGameUserSettings.h"

URuleOfTheGameUserSettings::URuleOfTheGameUserSettings()
	:bRealisticMap(false)
{

}

void URuleOfTheGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	const TCHAR* Section = TEXT("RuleOfTheGameUserSettings");
	GConfig->SetBool(Section, TEXT("sg.bRealisticMap"), bRealisticMap, GGameUserSettingsIni);
}

void URuleOfTheGameUserSettings::LoadSettings(bool bForceReload /*= false*/)
{
	Super::LoadSettings(bForceReload);

	const TCHAR* Section = TEXT("RuleOfTheGameUserSettings");
	GConfig->GetBool(Section, TEXT("sg.bRealisticMap"), bRealisticMap, GGameUserSettingsIni);
}

URuleOfTheGameUserSettings* URuleOfTheGameUserSettings::GetRuleOfTheGameUserSettings()
{
	return GEngine != NULL ? (Cast<URuleOfTheGameUserSettings>(GEngine->GetGameUserSettings())) : NULL;
}

bool URuleOfTheGameUserSettings::GetRealisticMap() const
{
	return bRealisticMap;
}

void URuleOfTheGameUserSettings::SetRealisticMap(bool NewValue)
{
	bRealisticMap = NewValue;
}
