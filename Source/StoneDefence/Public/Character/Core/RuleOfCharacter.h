// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Character/RuleCharacter.h"
#include "../StoneDefenceType.h"
#include "RuleOfCharacter.generated.h"

//class ATD_PlayerController;
class ATD_GameState;
class ATD_PlayerState;

UCLASS()
class STONEDEFENCE_API ARuleOfCharacter : public ACharacter , public IRuleCharacter
{
	GENERATED_BODY()

	//鼠标射线捕捉对象
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class UBoxComponent* TraceShowCharacterInformation;

	//开火点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class UArrowComponent* OpenFirePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* Widget;

	//跟踪点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class USceneComponent* HomingPoint;



public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorDeathDelegate);
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FActorDeathDelegate OnActorDeath;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class ADrawText> DrawTextClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = UI)
	FGuid GUID;

	//某些塔无死亡动画，死亡时播放特效，故需要一个延迟Destroy时间
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Death)
	float DeathDelayTime;

	UPROPERTY(EditDefaultsOnly, Category = Type)
	TEnumAsByte<EGameCharacterType::Type> CharacterType;

	// Sets default values for this character's properties
	ARuleOfCharacter();

	class ATD_PlayerController* GetGameController();

	class ATD_GameState* GetGameState();

	class ATD_PlayerState* GetPlayerState();


	//暴露给反射系统
	FORCEINLINE USceneComponent* GetHomingPoint() const { return HomingPoint; }
	FORCEINLINE UArrowComponent* GetFirePoint() const { return OpenFirePoint; }


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Attribute")
		bool IsActive() { return !IsDeath(); };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
		bool Isattack;//为了区别同名函数与变量，函数所有首字母大写，变量仅开头字母大写

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Test")
		UStaticMesh* GetDollMesh(FTransform& Transform);

	virtual EGameCharacterType::Type GetType();
	virtual bool IsDeath();
	virtual float GetHealth();
	virtual float GetMaxHealth();
	virtual ETeam GetTeamType();
	virtual FCharacterData& GetCharacterData();
	void UpdataUI();
	//该接口由服务器调用，而非客户端
	virtual void RegisterTeam();


	UFUNCTION()
		void AddSkillSlot_Client(const FGuid& SlotID);
	UFUNCTION()
		bool RemoveSkillSlot_Client(const FGuid& SlotID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool IsAttack();

	UFUNCTION()
	virtual void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

};
