// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Projectile/RuleOfProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/Core/RuleOfCharacter.h"
#include "Character/Core/RuleOfAIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "../StoneDefenceUtils.h"

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
	
	InitialLifeSpan = 4.0f;//��������Ϊ4�� 

	Spline = nullptr;
	CurrentSplineTime = 0.f;
	SplineOffset = 0.f;
	ChainAttackCount = 2;
}


// Called when the game starts or when spawned
void ARuleOfProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {//InstigatorCharacter -> InstigatorInOverlap
		if (ARuleOfAIController* InstigatorController = Cast<ARuleOfAIController>(InstigatorInOverlap->GetController())) {//OtherCharacter -> TakerInOverlap
			if (ARuleOfCharacter* TargetCharacter = InstigatorController->Target.Get()) {

				ProjectileCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARuleOfProjectile::BeginOverlap);

				switch (ProjectileType) {

					/*----------------------------------------------------------------------------------------*/
					/*--------------------------------------���⹥��-------------------------------------------*/
					/*----------------------------------------------------------------------------------------*/
					case EProjectileType::PROJECTILE_LASER:
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
						break;
					}


					/*----------------------------------------------------------------------------------------*/
					/*--------------------------------------���ٹ���-------------------------------------------*/
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
					/*--------------------------------------���ٹ���2------------------------------------------*/
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
					/*--------------------------------------ʵ������-------------------------------------------*/
					/*----------------------------------------------------------------------------------------*/
					case EProjectileType::PROJECTILE_BULLET:
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenFireParticle, GetActorLocation());
						break;
					}


					/*----------------------------------------------------------------------------------------*/
					/*---------------------------------�����ﷶΧ���������ף�-------------------------------------*/
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
					/*-------------------------------------��Χ����--------------------------------------------*/
					/*----------------------------------------------------------------------------------------*/
					case EProjectileType::PROJECTILE_RANGE:
					{
						ProjectileMovement->StopMovementImmediately();
						ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						RadialDamage(GetActorLocation(), Cast<ARuleOfCharacter>(GetInstigator()));
						break;
					}


					/*----------------------------------------------------------------------------------------*/
					/*-------------------------------------�׵繥��--------------------------------------------*/
					/*----------------------------------------------------------------------------------------*/
					case EProjectileType::PROJECTILE_LIGHTING:
					{
						ProjectileMovement->StopMovementImmediately();
						ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						UGameplayStatics::SpawnEmitterAttached(DamageParticle, TargetCharacter->GetHomingPoint());
						GetWorld()->GetTimerManager().SetTimer(ChainAttackHandle, this, &ARuleOfProjectile::ChainAttack,0.4f);
						break;
					}
				}
			}
		}
	}
}

void ARuleOfProjectile::BeginOverlap(UPrimitiveComponent* ObjectiveOverlappedComponent,/*�������ص������*/ //OverlappedComponent -> ObjectiveOverlappedComponent
									 AActor* SubjectiveActor,						  /*Ŀ��Actor*/      //OtherActor -> SubjectiveActor
									 UPrimitiveComponent* SubjectiveComp,			 /*Ŀ���ص����*/    //OtherComp -> OtherComp
									 int32 SubjectIndex,							/*Ŀ��ID*/         //OtherBodyIndex -> SubjectIndex
									 bool IsScanning,							   /*�Ƿ�ʼɨ��*/    //bFromSweep -> IsScanning
									 const FHitResult& ScanResult)				  /*ɨ����*/       //SweepResult -> ScanResult 
	/*--------------------------------------------------------------------------------------*/
{
	//�ж϶����Ƿ�ͬ��
	if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {//InstigatorCharacter -> InstigatorInOverlap
		if (ARuleOfCharacter* TakerInOverlap = Cast<ARuleOfCharacter>(SubjectiveActor)) {//OtherCharacter -> TakerInOverlap
			if (InstigatorInOverlap->GetTeamType() != TakerInOverlap->GetTeamType()) {
				if (TakerInOverlap->IsActive()) {

					//�����˺���Ч
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, ScanResult.Location);

					float DamageValue = Expression::GetDamage(InstigatorInOverlap, TakerInOverlap);
					switch (ProjectileType) {
						case EProjectileType::PROJECTILE_TRACKING:
						case EProjectileType::PROJECTILE_BULLET:
						case EProjectileType::PROJECTILE_TRACKING2:
						{
							UGameplayStatics::ApplyDamage(TakerInOverlap, DamageValue, InstigatorInOverlap->GetController(), InstigatorInOverlap, UDamageType::StaticClass());
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
								//����1Ϊ������Index��2Ϊ��Ч����λ�ã�3Ϊ����Index
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
							//���߳��� * ����ǰʱ�� / ��Ŀ����� / �ӵ����٣���
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
				Destroy();//�������Ŀ������������Ч�������ӵ�
			}
		}
	}
}

void ARuleOfProjectile::RadialDamage(const FVector& Origin, ARuleOfCharacter* InstigatorInOverlap) {
	if (InstigatorInOverlap) {
		TArray<AActor*> IgnoreActors;//����������
		//TArray<ARuleOfCharacter*> HostileActors;
		//������Ӻ��Զ����жԶ���
		for (TActorIterator<ARuleOfCharacter>it(GetWorld(), ARuleOfCharacter::StaticClass()); it; ++it) {
			if (ARuleOfCharacter* IgnoreTarget = *it) {
				FVector Distance = IgnoreTarget->GetActorLocation() - InstigatorInOverlap->GetActorLocation();//�������
				if (Distance.Size() <= 10000) {//���ڷ�Χ��
					if (IgnoreTarget->GetTeamType() == InstigatorInOverlap->GetTeamType()) {
						IgnoreActors.Add(IgnoreTarget);//��Ӻ��Զ���
					}
					else {
						//�����˺���Ч
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, IgnoreTarget->GetActorLocation());
						//HostileActors.Add(IgnoreTarget);//��ӵжԶ���
					}
				}
			}

		}

		UGameplayStatics::ApplyRadialDamageWithFalloff
		(GetWorld(),	                //������
			100.f, 10.f,				//�����˺�����С�˺�
			GetActorLocation(),			//��ը����
			400.f, 10000.f, 1.f,			//��ը�ڰ뾶����뾶��˥��ϵ��
			UDamageType::StaticClass(), //�˺�����
			IgnoreActors,				//����Ŀ��
			GetInstigator(),
			GetInstigator()->GetController(),
			ECollisionChannel::ECC_MAX);		//�˺������
		
	}
}

void ARuleOfProjectile::ChainAttack() {
	if (ChainAttackHandle.IsValid()) {//�����ʧЧ�������ʱ��
		GetWorld()->GetTimerManager().ClearTimer(ChainAttackHandle);
	}

	//����Ч��ʵ��
	if (ARuleOfCharacter* InstigatorInOverlap = Cast<ARuleOfCharacter>(GetInstigator())) {//InstigatorCharacter -> InstigatorInOverlap
		if (ARuleOfAIController* InstigatorController = Cast<ARuleOfAIController>(InstigatorInOverlap->GetController())) {//OtherCharacter -> TakerInOverlap
			if (ARuleOfCharacter* TargetCharacter = InstigatorController->Target.Get()) {
				UGameplayStatics::ApplyDamage(TargetCharacter,
												100.f,
												InstigatorInOverlap->GetController(),
												InstigatorInOverlap,
												UDamageType::StaticClass());
			}
		}
	}



	//��ʱ������
	ChainAttackCount--;//��ִ��3��
	if (ChainAttackCount > 0) {
		GetWorld()->GetTimerManager().SetTimer(ChainAttackHandle, this, &ARuleOfProjectile::ChainAttack, 0.4f);
	}
}
