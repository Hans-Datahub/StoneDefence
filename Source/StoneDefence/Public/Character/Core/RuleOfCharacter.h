// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../StoneDefenceType.h"
#include "Interface/Character/RuleCharacter.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Core/GameCore/TD_GameState.h"
#include "Core/GameCore/TD_PlayerState.h"
#include "RuleOfCharacter.generated.h"

UCLASS()
class STONEDEFENCE_API ARuleOfCharacter : public ACharacter , public IRuleCharacter
{
	GENERATED_BODY()

	//������߲�׽����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class UBoxComponent* TraceShowCharacterInformation;

	//�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class UArrowComponent* OpenFirePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttribute", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* Widget;

	//���ٵ�
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

	//ĳЩ������������������ʱ������Ч������Ҫһ���ӳ�Destroyʱ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Death)
	float DeathDelayTime;

	UPROPERTY(EditDefaultsOnly, Category = Type)
		TEnumAsByte<EGameCharacterType::Type> CharacterType;



public:
	// Sets default values for this character's properties
	ARuleOfCharacter();

	FORCEINLINE ATD_PlayerController* GetGameController(){
		return GetWorld() ? GetWorld()->GetFirstPlayerController<ATD_PlayerController>() : NULL;
	}
	FORCEINLINE ATD_GameState* GetGameState() {
		return GetWorld() ? GetWorld()-> GetGameState<ATD_GameState>(): NULL;
	}
	FORCEINLINE ATD_PlayerState* GetPlayerState() {
		return GetGameController()->GetPlayerState<ATD_PlayerState>();
	}
	//��¶������ϵͳ
	FORCEINLINE USceneComponent* GetHomingPoint() const { return HomingPoint; }
	FORCEINLINE UArrowComponent* GetFirePoint() const { return OpenFirePoint; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool IsAttack();

	UFUNCTION()
	virtual void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Attribute")
	bool IsActive() { return !IsDeath(); };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	bool Isattack;//Ϊ������ͬ�������������������������ĸ��д����������ͷ��ĸ��д

	UFUNCTION(Blueprintable, BlueprintPure, Category = "Towers|Test")
	UStaticMesh* GetDollMesh(FTransform& Transform);

	virtual EGameCharacterType::Type GetType();
	virtual bool IsDeath();
	virtual float GetHealth();
	virtual float GetMaxHealth();
	virtual ETeam GetTeamType();
	virtual FCharacterData& GetCharacterData();
	void UpdataUI();
	//�ýӿ��ɷ��������ã����ǿͻ���
	virtual void RegisterTeam();
};
