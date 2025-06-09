// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/UMG/UI_MainHall.h"
#include "Components/Button.h"
#include "UI/HallUI/UMG/UI_HallMenuSystem.h"
#include "UI_TutorialSystem.h"
#include "UI_ArchivesSystem.h"
#include "UI_GameSettingsSystem.h"
#include "Components/SizeBox.h"

template<class T,class UserObject>
UserObject* CreateAssistWidget(T* ThisClass, UClass* AssistClass, USizeBox* WidgetArray) {
    UserObject* UserObjectElement = nullptr;

    //播放动画的判断
    if (0) {
        //播放  淡入
    }

    //如果容器内已有目标类，清除。若无目标类，清除其余子类
    if (WidgetArray->GetChildAt(0)) {
        if (WidgetArray->GetChildAt(0)->IsA(AssistClass)) {
            
            //关闭Board 淡出

            return UserObjectElement;
        }
        else {
            WidgetArray->ClearChildren();
        }
    }

    UserObjectElement = CreateWidget<UserObject>(ThisClass->GetWorld(), AssistClass);

    if (UserObjectElement) {
        if (WidgetArray->AddChild(UserObjectElement)) {

        }
        else {UserObjectElement->RemoveFromParent();}   
    }

    return UserObjectElement;
}

void UUI_MainHall::NativeConstruct(){
    Super::NativeConstruct();

    if (HallMenuSystem) {

        //Game Start
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::GameStart);
            HallMenuSystem->BindGameStart(Delegate);
        }

        //Secret Territory
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::SecretTerritory);
            HallMenuSystem->BindSecretTerritory(Delegate);
        }

        //History
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::History);
            HallMenuSystem->BindHistory(Delegate);
        }

        //Game Settings
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::GameSettings);
            HallMenuSystem->BindGameSettings(Delegate);
        }

        //Tutorial Website
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::TutorialWebsite);
            HallMenuSystem->BindTutorialWebsite(Delegate);
        }

        //Browser
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::Browser);
            HallMenuSystem->BindBrowser(Delegate);
        }

        //Special Content
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::SpecialContent);
            HallMenuSystem->BindSpecialContent(Delegate);
        }

        //Quit Game
        {
            FOnButtonClickedEvent Delegate;
            Delegate.AddDynamic(this, &UUI_MainHall::QuitGame);
            HallMenuSystem->BindQuitGame(Delegate);
        }
    }

}


void UUI_MainHall::GameStart() {
    UGameplayStatics::OpenLevel(GetWorld(), "SelectMap");
}
void UUI_MainHall::SecretTerritory() {

}
void UUI_MainHall::History() {
    CreateAssistWidget<UUI_MainHall, UUI_ArchivesSystem>(this, ArchivesSystemClass, BoxList);
}
void UUI_MainHall::GameSettings() {
    CreateAssistWidget<UUI_MainHall, UUI_GameSettingsSystem>(this, GameSettingsClass, BoxList);
}
void UUI_MainHall::TutorialWebsite() {
    CreateAssistWidget<UUI_MainHall, UUI_TutorialSystem>(this, TutorialSystemClass, BoxList);
}
void UUI_MainHall::Browser() {

}
void UUI_MainHall::SpecialContent() {

}
void UUI_MainHall::QuitGame() {

}