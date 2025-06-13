// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Core/RuleOfCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Character/UI_Health.h"
#include "../StoneDefenceUtils.h"
#include "Actor/DrawText.h"
#include "Components/StaticMeshComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h" 
#include "Particles/ParticleEmitter.h"  

#include "../StoneDefenceMacro.h"
#include "Character/Projectile/RuleOfProjectile.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Character/Damage/RuleOfDamag.h"

// Sets default values
ARuleOfCharacter::ARuleOfCharacter()
	:Isattack(false)
{
	GUID = FGuid::NewGuid();
	PrimaryActorTick.bCanEverTick = true;
	HomingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPoint"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	OpenFirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	TraceShowCharacterInformation = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceBox"));

	HomingPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	OpenFirePoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TraceShowCharacterInformation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	DeathDelayTime = 10.f;

	//设置预设Scanning
	TraceShowCharacterInformation->SetCollisionProfileName("Scanning");
	TraceShowCharacterInformation->SetBoxExtent(FVector(38, 38, 100));

	CharacterType = EGameCharacterType::MINI;
}

ATD_PlayerController* ARuleOfCharacter::GetGameController()
{
	return GetWorld() ? GetWorld()->GetFirstPlayerController<ATD_PlayerController>() : nullptr;
}

ATD_GameState* ARuleOfCharacter::GetGameState()
{
	return GetWorld() ? GetWorld()->GetGameState<ATD_GameState>() : nullptr;
}

ATD_PlayerState* ARuleOfCharacter::GetPlayerState()
{
	return GetGameController()->GetPlayerState<ATD_PlayerState>();
}

// Called when the game starts or when spawned
void ARuleOfCharacter::BeginPlay()
{
	Super::BeginPlay();

	TraceShowCharacterInformation->OnClicked.AddDynamic(this, &ARuleOfCharacter::OnClicked);
	if (!GetController()) {
		SpawnDefaultController();
	}

}

// Called every frame
void ARuleOfCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdataUI();
}

EGameCharacterType::Type ARuleOfCharacter::GetType() {
	return CharacterType;
}


float ARuleOfCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser){
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	float DamageValue = 0.f;
	if (URuleOfDamage* DamageClass = DamageEvent.DamageTypeClass->GetDefaultObject<URuleOfDamage>()) {
		if (const FSkillData* SkillData = DamageClass->SkillData) {

			auto DrawGameText = [&](ARuleOfCharacter* InOwner, const TCHAR* InText, float Number, FLinearColor Color) {
				if (DrawTextClass) {
					if (ADrawText* Text = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, InOwner->GetActorLocation(), FRotator::ZeroRotator)) {
						int32 IntNumber = static_cast<int32>(Number);//去小数

						//生成数组供Format函数使用
						TArray<FStringFormatArg>NumberArray;
						NumberArray.Add((FStringFormatArg)IntNumber);

						FString IntText = FString::Format(InText, NumberArray);
						Text->SetTextBlock(IntText, Color, Number);
					}
				}
			};


			

			if (SkillData->SkillType.SkillBoostType == ESkillBoostType::SUBTRACT) {
				DamageValue = Expression::GetDamage(Cast<ARuleOfCharacter>(DamageCauser), this);
				if (Damage) {
					GetCharacterData().Health -= Damage;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Red);
				}

				//若为攻击减益
				if (SkillData->PhysicalAttackModify) {
					GetCharacterData().PhysicalAttack -= SkillData->PhysicalAttackModify;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Blue);
				}

				//若为防御减益
				if (SkillData->ArmorModify) {
					GetCharacterData().Armor -= SkillData->ArmorModify;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::White);
				}

				//若为攻击速度减益
				if (SkillData->AttackSpeedModify) {
					GetCharacterData().AttackSpeed -= SkillData->AttackSpeedModify;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Yellow);
				}

				//若为生命值减益
				if (SkillData->HealthModify) {
					GetCharacterData().Health -= SkillData->HealthModify;
					if (GetCharacterData().Health > GetCharacterData().MaxHealth) {//若加血后超过最大健康，修正
						GetCharacterData().Health = GetCharacterData().MaxHealth
					}
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Green);
				}
			}
			else if(SkillData->SkillType.SkillBoostType == ESkillBoostType::ADD){
				//若为攻击增益
				if (SkillData->PhysicalAttackModify) {
					GetCharacterData().PhysicalAttack+= SkillData->PhysicalAttackModify;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Blue);
				}

				//若为防御增益
				if (SkillData->ArmorModify) {
					GetCharacterData().Armor += SkillData->ArmorModify;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::White);
				}

				//若为攻击速度增益
				if (SkillData->AttackSpeedModify) {
					GetCharacterData().AttackSpeed += SkillData->AttackSpeedModify;
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Yellow);
				}

				//若为生命值增益
				if (SkillData->HealthModify) {
					GetCharacterData().Health += SkillData->HealthModify;
					if (GetCharacterData().Health > GetCharacterData().MaxHealth) {//若加血后超过最大健康，修正
						GetCharacterData().Health = GetCharacterData().MaxHealth
					}
					//绘制伤害数字
					DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Green);
				}
			}

			//死亡后执行相关操作
			if (!IsActive()) {
				if (ATD_PlayerState* TempPlayerState = Cast<ATD_PlayerState>(GetPlayerState())) {		
					FPlayerData PlayerData = TempPlayerState->GetPlayerData();
					if (PlayerData.IsValid()) {
						if (PlayerData.Team != GetTeamType()) {
							PlayerData.GameGold += GetCharacterData().Gold;
						}
					}
				}




				GetCharacterData().Health = 0.0f;
				SetLifeSpan(DeathDelayTime);
				Widget->SetVisibility(false);

				if (ARuleOfCharacter* CauserCharacter = Cast<ARuleOfCharacter>(DamageCauser)) {
					if (CauserCharacter->IsActive()) {
						if (CauserCharacter->GetCharacterData().UpdateExp(GetCharacterData().AddEmpiricalValue)) {
							//播放升级特效
						}
						DrawGameText(CauserCharacter, TEXT("+Exp {0}"), GetCharacterData().AddEmpiricalValue, FLinearColor::Yellow);
					}
				}

				//寻找范围内最近的敌人
				TArray<ARuleOfCharacter*> EnemyCharacters;
				StoneDefenceUtils::FindMostClosedTargetInRange(this, 1000.f, EnemyCharacters);
				for (ARuleOfCharacter* InEnemy : EnemyCharacters) {
					if (InEnemy != DamageCauser) {
						if (InEnemy->IsActive()) {
							if (InEnemy->GetCharacterData().UpdateExp(GetCharacterData().AddEmpiricalValue * 0.3f)) {

							}
							DrawGameText(InEnemy, TEXT("+Exp {0}"), GetCharacterData().AddEmpiricalValue, FLinearColor::Yellow);
						}
					}
				}

				GetGameState()->RemoveCharacterData(GUID);
			}

		}
	}

	return DamageValue;
}

void ARuleOfCharacter::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) {
	TD_PRINT_S("Actor Clicked");
}



bool ARuleOfCharacter::IsDeath() {
	return GetHealth() <= 0;
}

float ARuleOfCharacter::GetHealth() {
	return GetCharacterData().Health;
}

float ARuleOfCharacter::GetMaxHealth() {
	return GetCharacterData().GetMaxHealth();
}

ETeam ARuleOfCharacter::GetTeamType() {
	return GetCharacterData().Team;
}

bool ARuleOfCharacter::IsAttack() {
	return false;
}

void ARuleOfCharacter::RegisterTeam() {
}

FCharacterData& ARuleOfCharacter::GetCharacterData() {
#if WITH_EDITOR
	if (GetGameState()) {
		return GetGameState()->GetCharacterData(GUID);
	}
	return GetGameState()->GetCharacterDataNULL();
#else
	return GetGameState()->GetCharacterDataNULL();
#endif
}

void ARuleOfCharacter::UpdataUI() {
	if (Widget) {
		if(GetCharacterData().IsValid()){
			if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject())) {
				HealthUI->SetTitle(GetCharacterData().Name.ToString());
				HealthUI->SetHealth(GetHealth() / GetMaxHealth());
			}
		}	
	}
}


UStaticMesh* ARuleOfCharacter::GetDollMesh(FTransform& Transform) {
	TArray<USceneComponent*> SceneComponent;
	RootComponent->GetChildrenComponents(true, SceneComponent);
	//三种获取Mesh的方法
	for (auto& OuterTemp : SceneComponent) {
		if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(OuterTemp)) {
			if (MeshComponent->GetStaticMesh()) {//加该判断后，这个接口就会一直循环直到有Mesh才返回，而不是直接返回空值
				Transform = MeshComponent->GetComponentTransform();
				return MeshComponent->GetStaticMesh();
			}
		}
		else if(UParticleSystemComponent* ParticleSystemComponent = Cast<UParticleSystemComponent>(OuterTemp)){
			if (UStaticMesh* TempMesh = MeshUtils::ParticleSystemComponentToStaticMesh(ParticleSystemComponent)) {
				Transform = ParticleSystemComponent->GetComponentTransform();
				return TempMesh;
			}
		}
		//后面建立缓存池之后会再次回到此处进行改动
		else if(USkeletalMeshComponent* SkeletalComponent = Cast<USkeletalMeshComponent>(OuterTemp)){
			Transform = SkeletalComponent->GetComponentTransform();
			SkeletalComponent->SetRelativeTransform(FTransform::Identity);
			SkeletalComponent->SetWorldTransform(FTransform::Identity);
			SkeletalComponent->SetRelativeRotation(Transform.GetRotation());
			if (UStaticMesh* NewMesh = MeshUtils::SkeletalMeshComponentToStaticMesh(SkeletalComponent)) {
				return NewMesh;
			}
		}
	}
	return NULL;
}


void ARuleOfCharacter::AddSkillSlot_Client(const FGuid& SlotID) {
	if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject()))
		HealthUI->AddTakenSkillSlot(SlotID);
}

bool ARuleOfCharacter::RemoveSkillSlot_Client(const FGuid& SlotID) {
	if (UUI_Health* HealthUI = Cast<UUI_Health>(Widget->GetUserWidgetObject())) {
		HealthUI->RemoveTakenSkillSlot(SlotID);
		return true;
	}
	return false;
}

void ARuleOfCharacter::ResetGUID() {
	GUID = FGuid::NewGuid();
} 

void ARuleOfCharacter::InitPassiveSkill() {// = InitSkill
	for (auto& Temp : PassiveSkillIDArray) {
		if (ARuleOfProjectile* Projectile = StoneDefenceUtils::SpawnProjectile(GetWorld(), this, Temp, OpenFirePoint->GetComponentLocation(), OpenFirePoint->GetComponentRotation())) {
			Projectile->InitSkill();
		}
	}
}

void ARuleOfCharacter::UpdatePassiveSkill(int32 SkillID) {// = UpdateSkill
	for (auto& Temp : PassiveSkillIDArray) {
		if (Temp == SkillID) {
			if (ARuleOfProjectile* Projectile = StoneDefenceUtils::SpawnProjectile(GetWorld(), this, Temp, OpenFirePoint->GetComponentLocation(), OpenFirePoint->GetComponentRotation())) {
				Projectile->InitSkill();
			}
			break;
		}
	}
}



