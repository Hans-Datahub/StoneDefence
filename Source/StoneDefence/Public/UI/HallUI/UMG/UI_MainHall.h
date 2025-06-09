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

public:
	//UUserWidget ��һ���麯���������� UI ���������ʱִ�г�ʼ���߼���
	virtual void NativeConstruct();


	//�˴��ĺ�������������HallMenuSystem�еģ����ǻ����������ø����ٵ������ѭ��
	//����ֱ�Ӱ�������ʵ��ֱ��Ų��������
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
