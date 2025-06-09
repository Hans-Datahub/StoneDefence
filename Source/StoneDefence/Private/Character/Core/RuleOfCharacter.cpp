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

	//����Ԥ��Scanning
	TraceShowCharacterInformation->SetCollisionProfileName("Scanning");
	TraceShowCharacterInformation->SetBoxExtent(FVector(38, 38, 100));

	CharacterType = EGameCharacterType::MINI;
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

	auto DrawGameText = [&](ARuleOfCharacter* InOwner, const TCHAR* InText, float Number, FLinearColor Color) {
		if (DrawTextClass) {
			if (ADrawText* Text = GetWorld()->SpawnActor<ADrawText>(DrawTextClass, InOwner->GetActorLocation(), FRotator::ZeroRotator)) {
				int32 IntNumber = static_cast<int32>(Number);//ȥС��

				//�������鹩Format����ʹ��
				TArray<FStringFormatArg>NumberArray;
				NumberArray.Add((FStringFormatArg)IntNumber);

				FString IntText = FString::Format(InText, NumberArray);
				Text->SetTextBlock(IntText, Color, Number);
			}
		}
	};

	float DamageValue = Expression::GetDamage(Cast<ARuleOfCharacter>(DamageCauser), this);

	GetCharacterData().Health -= Damage / 10.f;

	//������ִ����ز���
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
					//����������Ч
				}
				DrawGameText(CauserCharacter, TEXT("+Exp {0}"), GetCharacterData().AddEmpiricalValue, FLinearColor::Yellow);
			}
		}

		//Ѱ�ҷ�Χ������ĵ���
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
	//�����˺�����
	DrawGameText(this, TEXT("-{0}"), Damage, FLinearColor::Red);

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
	//���ֻ�ȡMesh�ķ���
	for (auto& OuterTemp : SceneComponent) {
		if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(OuterTemp)) {
			if (MeshComponent->GetStaticMesh()) {//�Ӹ��жϺ�����ӿھͻ�һֱѭ��ֱ����Mesh�ŷ��أ�������ֱ�ӷ��ؿ�ֵ
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
		//���潨�������֮����ٴλص��˴����иĶ�
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