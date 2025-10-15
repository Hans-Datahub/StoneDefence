// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleGameUserSettings.h"
#include "RuleOfTheGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class STONEDEFENCE_API URuleOfTheGameUserSettings : public USimpleGameUserSettings
{
	GENERATED_BODY()
	
	//±≥æ∞“Ù¿÷
	UPROPERTY(config)
	bool bRealisticMap;

public:
	URuleOfTheGameUserSettings();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides)override;
	virtual void LoadSettings(bool bForceReload = false) override;

	static URuleOfTheGameUserSettings* GetRuleOfTheGameUserSettings();

	UFUNCTION(BlueprintPure, Category = "RuleOfTheGameUserSettings")
	bool GetRealisticMap() const;

	UFUNCTION(BlueprintCallable,Category = "RuleOfTheGameUserSettings")
	void SetRealisticMap(bool NewValue);
};
