// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Projectile/RuleOfProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/Core/RuleOfAIController.h"
#include "Character/Projectile/RuleOfProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "../StoneDefenceUtils.h"
#include "Character/Damage/RuleOfDamage.h"

// Sets default values
ARuleOfProjectile::ARuleOfProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision")); //BoxDamage -> ProjectileCollision
	RootProjectile = CreateDefaultSubobject<USceneComponent>(TEXT("RootProjectile"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	RootComponent = RootProjectile;
	ProjectileCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->InitialSpeed = 1600.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->UpdatedComponent = RootComponent;

	ProjectileType = EProjectileType::PROJECTILE_RANGE;
	
	InitialLifeSpan = 4.0f;//声明周期为4秒 

	Spline = nullptr;
	CurrentSplineTime = 0.f;
	SplineOffset = 0.f;
	ChainAttackCount = 2;

	SkillID = INDEX_NONE;
	SubmissionSkillRequestType = ESubmissionSkillRequestType::AUTO;
}


// Called when the game starts or when spawned
void ARuleOfProjectile::BeginPlay()
{
	Super::BeginPlay();

	//12-4 14:25 处对比发现有该代码。补充
	//ProjectileCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARuleOfProjectile::BeginOverlap);
}

void ARuleOfProjectile::InitSkill() {
	if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {//InstigatorCharacter -> InstigatorInOverlap
		if (ARuleOfAIController* InstigatorController = Cast<ARuleOfAIController>(InstigatorInOverlap->GetController())) {//OtherCharacter -> TakerInOverlap
			if (ARuleOfCharacter* TargetCharacter = InstigatorController->Target.Get()) {

				ProjectileCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARuleOfProjectile::BeginOverlap);

				switch (ProjectileType) {

					/*----------------------------------------------------------------------------------------*/
					/*--------------------------------------激光攻击-------------------------------------------*/
					/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_LASER:
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
					break;
				}


				/*----------------------------------------------------------------------------------------*/
				/*--------------------------------------跟踪攻击-------------------------------------------*/
				/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_TRACKING:
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
					ProjectileMovement->bIsHomingProjectile = true;
					ProjectileMovement->bRotationFollowsVelocity = true;
					ProjectileMovement->HomingAccelerationMagnitude = 4000.f;
					ProjectileMovement->HomingTargetComponent = TargetCharacter->GetHomingPoint();
					break;
				}


				/*----------------------------------------------------------------------------------------*/
				/*--------------------------------------跟踪攻击2------------------------------------------*/
				/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_TRACKING2:
				{
					//ProjectileMovement->StopMovementImmediately();
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());

					Spline = NewObject<USplineComponent>(this, TEXT("SplineInstance"));
					Spline->RegisterComponent();

					Spline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::Local);
					FVector DistanceVector = InstigatorInOverlap->GetActorLocation() - TargetCharacter->GetActorLocation();
					FVector Position = (DistanceVector / 2) + TargetCharacter->GetActorLocation();
					Position.Y += SplineOffset;
					Position.Z = (DistanceVector.Size() / 2.f) * 0.5f;
					Spline->SetLocationAtSplinePoint(1, Position, ESplineCoordinateSpace::Local);
					Spline->AddSplinePoint(TargetCharacter->GetActorLocation(), ESplineCoordinateSpace::Local);
					break;
				}


				/*----------------------------------------------------------------------------------------*/
				/*--------------------------------------实弹攻击-------------------------------------------*/
				/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_BULLET:
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
					break;
				}


				/*----------------------------------------------------------------------------------------*/
				/*---------------------------------抛射物范围攻击（手雷）-------------------------------------*/
				/*----------------------------------------------------------------------------------------*/

				case EProjectileType::PROJECTILE_GRENADE:
				{
					ProjectileMovement->StopMovementImmediately();
					ProjectileMovement->ProjectileGravityScale = 0.f;

					FVector Distance = TargetCharacter->GetActorLocation() - GetActorLocation();

					float ProjectileFlyTime = (Distance.Size() / ProjectileMovement->InitialSpeed);
					float YSpeed = ProjectileMovement->GetGravityZ() * ProjectileFlyTime;
					float XSpeed = ProjectileMovement->InitialSpeed * ProjectileFlyTime;
					float Speed = FMath::Sqrt(XSpeed * XSpeed + YSpeed * YSpeed);

					float CosRadian = FMath::Acos(Distance.Size() / Speed * (ProjectileFlyTime * (PI * 0.1f)));
					FRotator Rot = GetActorRotation();
					Rot.Pitch = CosRadian * (180 / PI);
					SetActorRotation(Rot);

					ProjectileMovement->SetVelocityInLocalSpace(FVector(1.f, 0.f, 0.f) * ProjectileMovement->InitialSpeed);

					RadialDamage(GetActorLocation(), Cast<ARuleOfCharacter>(GetInstigator()));
					break;
				}


				/*----------------------------------------------------------------------------------------*/
				/*-------------------------------------范围攻击--------------------------------------------*/
				/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_RANGE:
				{
					ProjectileMovement->StopMovementImmediately();
					ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					RadialDamage(GetActorLocation(), Cast<ARuleOfCharacter>(GetInstigator()));
					break;
				}


				/*----------------------------------------------------------------------------------------*/
				/*-------------------------------------雷电攻击--------------------------------------------*/
				/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_LIGHTING:
				{
					ProjectileMovement->StopMovementImmediately();
					ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					UGameplayStatics::SpawnEmitterAttached(DamageParticle, TargetCharacter->GetHomingPoint());
					GetWorld()->GetTimerManager().SetTimer(ChainAttackHandle, this, &ARuleOfProjectile::ChainAttack, 0.4f);

					SubmissionSkillRequest();
					break;
				}

				/*----------------------------------------------------------------------------------------*/
				/*--------------------------------------无类型---------------------------------------------*/
				/*----------------------------------------------------------------------------------------*/
				case EProjectileType::PROJECTILE_NONE:
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
					ProjectileMovement->StopMovementImmediately();
					ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					SubmissionSkillRequest();
					break;
				}
				}
			}
		}
	}

}


void ARuleOfProjectile::BeginOverlap(UPrimitiveComponent* ObjectiveOverlappedComponent,/*主体所重叠的组件*/ //OverlappedComponent -> ObjectiveOverlappedComponent
									 AActor* SubjectiveActor,						  /*目标Actor*/      //OtherActor -> SubjectiveActor
									 UPrimitiveComponent* SubjectiveComp,			 /*目标重叠组件*/    //OtherComp -> OtherComp
									 int32 SubjectIndex,							/*目标ID*/         //OtherBodyIndex -> SubjectIndex
									 bool IsScanning,							   /*是否开始扫描*/    //bFromSweep -> IsScanning
									 const FHitResult& ScanResult)				  /*扫描结果*/       //SweepResult -> ScanResult 
	/*--------------------------------------------------------------------------------------*/
{
	if (const FSkillData* InData = GetSkillData()) {
		//判断二者是否同队
		if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {//InstigatorCharacter -> InstigatorInOverlap
			if (ARuleOfCharacter* TakerInOverlap = Cast<ARuleOfCharacter>(SubjectiveActor)) {//OtherCharacter -> TakerInOverlap
				auto VerifyConsistency = [&]()-> bool {
					bool bVerifyConsistency = false;
					if (InData->SkillType.SkillTargetType == ESkillTargetType::ALLIES) {
						bVerifyConsistency = InstigatorInOverlap->GetTeamType() == TakerInOverlap->GetTeamType();
					}
					else if (InData->SkillType.SkillTargetType == ESkillTargetType::ENEMY){
						bVerifyConsistency = InstigatorInOverlap->GetTeamType() != TakerInOverlap->GetTeamType();
					}
					return bVerifyConsistency;
				};



				if (VerifyConsistency()) {
					if (TakerInOverlap->IsActive()) {

						//生成伤害特效
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, ScanResult.Location);

						float DamageValue = Expression::GetDamage(InstigatorInOverlap, TakerInOverlap);
						switch (ProjectileType) {
							case EProjectileType::PROJECTILE_TRACKING:
							case EProjectileType::PROJECTILE_BULLET:
							case EProjectileType::PROJECTILE_TRACKING2:
							{
								UClass* RuleOfDamage = URuleOfDamage::StaticClass();
								if (URuleOfDamage* DamageClass = RuleOfDamage->GetDefaultObject<URuleOfDamage>())
									DamageClass->SkillData = InData;//设置伤害类型的CDO

								UGameplayStatics::ApplyDamage(TakerInOverlap, DamageValue, InstigatorInOverlap->GetController(), InstigatorInOverlap, RuleOfDamage);
								SubmissionSkillRequest();
								Destroy();
								break;
							}

							case EProjectileType::PROJECTILE_RANGE:
							{
								RadialDamage(TakerInOverlap->GetActorLocation(), InstigatorInOverlap);
								Destroy();
								break;
							}
						}
					}
				}
			}
		}
	}
}

// Called every frame
void ARuleOfProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {
		if (ARuleOfAIController* InstigatorController = Cast<ARuleOfAIController>(InstigatorInOverlap->GetController())) {
			if (ARuleOfCharacter* TargetCharacter = InstigatorController->Target.Get()) {

				switch (ProjectileType) {
					case EProjectileType::PROJECTILE_LIGHTING: {
						TArray<USceneComponent*> SceneComponent;
						RootComponent->GetChildrenComponents(true, SceneComponent);
						for (auto& Temp : SceneComponent) {
							if (UParticleSystemComponent* ParticleSystem = Cast<UParticleSystemComponent>(Temp)) {
								//参数1为发射器Index，2为特效发生位置，3为光束Index
								ParticleSystem->SetBeamTargetPoint(0, TargetCharacter->GetHomingPoint()->GetComponentLocation(), 0);
								ParticleSystem->SetBeamSourcePoint(0, InstigatorInOverlap->GetFirePoint()->GetComponentLocation(), 0);
								break;
							}
						}
						break;
					}
					case EProjectileType::PROJECTILE_TRACKING2: {
						if (Spline) {
							FVector DistanceVector = TargetCharacter->GetActorLocation() - InstigatorInOverlap->GetActorLocation();
							CurrentSplineTime += DeltaTime;
							//曲线长度 * （当前时间 / （目标距离 / 子弹初速））
							float Distance = Spline -> GetSplineLength() * (CurrentSplineTime / (DistanceVector.Size() / 1000.f));
							FVector Location = Spline->GetWorldLocationAtDistanceAlongSpline(Distance);
							FRotator Rotation = Spline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
							SetActorLocationAndRotation(Location, Rotation);

							if ((Location - TargetCharacter->GetActorLocation()).Size() <= 100.f) {
								FHitResult ScanResult;
								ScanResult.Location = Location;
								BeginOverlap(nullptr, TargetCharacter, nullptr, 0, false, ScanResult);
								Destroy();
							}
						}
						break;
					}
				}
				if (!TargetCharacter->IsActive()) {
					Destroy();
				}
			}
			else {
				Destroy();//若所获得目标死亡或者无效，销毁子弹
			}
		}
	}
}

void ARuleOfProjectile::RadialDamage(const FVector& Origin, ARuleOfCharacter* InstigatorInOverlap) {
	if (InstigatorInOverlap) {
		if (const FSkillData* InData = GetSkillData()) {
			auto SpawnEffect = [&](ARuleOfCharacter* NewCharacter) {
				//生成伤害特效
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, NewCharacter->GetActorLocation());
				//提交技能申请
				SubmissionSkillRequest();
			};


			TArray<AActor*> IgnoreActors;//建立忽略组
			//TArray<ARuleOfCharacter*> HostileActors;
			//迭代添加忽略对象或敌对对象
			for (TActorIterator<ARuleOfCharacter>it(GetWorld(), ARuleOfCharacter::StaticClass()); it; ++it) {
				if (ARuleOfCharacter* IgnoreTarget = *it) {
					FVector Distance = IgnoreTarget->GetActorLocation() - InstigatorInOverlap->GetActorLocation();//计算距离
					if (Distance.Size() <= InData->AttackRange) {//若在范围内
						if (InData->SkillType.SkillTargetType == ESkillTargetType::ALLIES) {
							if (IgnoreTarget->GetTeamType() == InstigatorInOverlap->GetTeamType()) {
								SpawnEffect(IgnoreTarget);
							}
							else {
								IgnoreActors.Add(IgnoreTarget);
							}
						}
						else if (InData->SkillType.SkillTargetType == ESkillTargetType::ENEMY) {
							if (IgnoreTarget->GetTeamType() != InstigatorInOverlap->GetTeamType()) {
								SpawnEffect(IgnoreTarget);
							}
							else {
								IgnoreActors.Add(IgnoreTarget);
							}
						}
					}
				}

			}

			UClass* RuleOfDamage = URuleOfDamage::StaticClass();
			if (URuleOfDamage* DamageClass = RuleOfDamage->GetDefaultObject<URuleOfDamage>()) {
				DamageClass->SkillData = InData;//设置伤害类型的CDO

				UGameplayStatics::ApplyRadialDamageWithFalloff
				(GetWorld(),	                //上下文
					100.f, 10.f,				//基础伤害，最小伤害
					GetActorLocation(),			//爆炸中心
					400.f, 10000.f, 1.f,			//爆炸内半径，外半径，衰减系数
					RuleOfDamage, //伤害类型
					IgnoreActors,				//忽略目标
					GetInstigator(),
					GetInstigator()->GetController(),
					ECollisionChannel::ECC_MAX);		//伤害输出者

			}

		}	
	}
}

void ARuleOfProjectile::SubmissionSkillRequest()
{
	if (SkillID != INDEX_NONE) {
		if (ARuleOfCharacter* InstigatorCharacter = Cast<ARuleOfCharacter>(GetInstigator())) {
			if (ATD_GameState* InGameState = GetWorld()->GetGameState<ATD_GameState>()) {
				const FCharacterData& CharacterData = InstigatorCharacter->GetCharacterData();
				if (CharacterData.IsValid()) {
					if (!InGameState->IsVerificationSkillTemplate(CharacterData, SkillID)) {
						//提交
						InGameState->AddSkillDataTemplateToCharacterData(InstigatorCharacter->GUID, SkillID);

						if(SubmissionSkillRequestType == ESubmissionSkillRequestType::MANUAL) {
							//根据子弹数据提交类型 设置其所属技能的 数据提交类型
							InGameState->SetSubmissionDataType(InstigatorCharacter->GUID, SkillID, SubmissionSkillRequestType);

						}
					}
				}
			}
		}
	}
}

const FSkillData* ARuleOfProjectile::GetSkillData()
{
	if (ATD_GameState* InGameState = GetWorld()->GetGameState<ATD_GameState>()) {
		return InGameState->GetSkillData(SkillID);
	}
	return nullptr;
}

void ARuleOfProjectile::ChainAttack() {
	if (ChainAttackHandle.IsValid()) {//若句柄失效，清除计时器
		GetWorld()->GetTimerManager().ClearTimer(ChainAttackHandle);
	}

	//攻击效果实现
	if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {//InstigatorCharacter -> InstigatorInOverlap
		if (ARuleOfAIController* InstigatorController = Cast<ARuleOfAIController>(InstigatorInOverlap->GetController())) {//OtherCharacter -> TakerInOverlap
			if (ARuleOfCharacter* TargetCharacter = InstigatorController->Target.Get()) {

				if (const FSkillData* InData = GetSkillData()) {
					UClass* RuleOfDamage = URuleOfDamage::StaticClass();
					if (URuleOfDamage* DamageClass = RuleOfDamage->GetDefaultObject<URuleOfDamage>()) {
						DamageClass->SkillData = InData;//设置伤害类型的CDO

						UGameplayStatics::ApplyDamage(TargetCharacter,
							100.f,
							InstigatorInOverlap->GetController(),
							InstigatorInOverlap,
							UDamageType::StaticClass());
					}

				}
			}
		}
	}



	//计时器重置
	ChainAttackCount--;//共执行3次
	if (ChainAttackCount > 0) {
		GetWorld()->GetTimerManager().SetTimer(ChainAttackHandle, this, &ARuleOfProjectile::ChainAttack, 0.4f);
	}
}

void ARuleOfProjectile::ResetIteration() {
	ProjectileType = EProjectileType::PROJECTILE_NONE;
	InitSkill();
}
