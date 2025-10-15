// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_RuleOfWidget.h"
#include "UI_MainHall.generated.h"

/**
 * 
 */
UCLASS()
class STONEDEFENCE_API UUI_MainHall : public UUI_RuleOfWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UBorder* MainBoard;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* BoxList;

	UPROPERTY(meta = (BindWidget))
	class UUI_HallMenuSystem* HallMenuSystem;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_ArchivesSystem> ArchivesSystemClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_GameSettingsSystem> GameSettingsClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_TutorialSystem> TutorialSystemClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_SimplePopup> PopupClass;

public:
	//UUserWidget 的一个虚函数，用于在 UI 组件被构造时执行初始化逻辑。
	virtual void NativeConstruct();


	//此处的函数本来是做到HallMenuSystem中的，但是会造成子类调用父类再调子类的循环
	//所以直接把声明和实现直接挪到父类中
	UFUNCTION()
		void GameStart();
	UFUNCTION()
		void SecretTerritory();
	UFUNCTION()
		void History();
	UFUNCTION()
		void GameSettings();
	UFUNCTION()
		void TutorialWebsite();
	UFUNCTION()
		void Browser();
	UFUNCTION()
		void SpecialContent();
	UFUNCTION()
		void QuitGame();
};
