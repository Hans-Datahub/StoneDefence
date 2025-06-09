// Copyright Epic Games, Inc. All Rights Reserved.


#include "TD_GameMode.h"
#include "Core/GameCore/TD_GameState.h"
#include "Core/GameCore/TD_PlayerState.h"
#include "Core/GameCore/TD_PlayerController.h"
#include "Core/GameCore/TD_GameCamera.h"
#include "UI/GameUI/Core/RuleOfHUD.h"
#include "../StoneDefenceUtils.h"
#include "Item/SpawnPoint.h"
#include "Character/CharacterCore/Monsters.h"
#include "Engine/World.h"




ATD_GameMode::ATD_GameMode()
{
	//将UE中的pawn，	contorller，PlayerState等数据与C++代码绑定起来
	GameStateClass = ATD_GameState::StaticClass();
	PlayerControllerClass = ATD_PlayerController::StaticClass();
	DefaultPawnClass = ATD_GameCamera::StaticClass();
	PlayerStateClass = ATD_PlayerState::StaticClass();
	HUDClass = ARuleOfHUD::StaticClass();
}

void ATD_GameMode::BeginPlay() {
	Super::BeginPlay();
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {
		TempGameState->GetGameData().AssignedMonsterAmount();//分配每波怪物数量
	}

	//if (1) {
	//	SaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));
	//}
}

void ATD_GameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);


	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {

		CallUpdateAllClient([&](ATD_PlayerController* MyPlayerController) {
				if (ATD_PlayerState* PlayerState = MyPlayerController->GetPlayerState<ATD_PlayerState>()) {
					//每1.25秒+1金币
					PlayerState->GetPlayerData().GameGoldTime += DeltaSeconds;
					if (PlayerState->GetPlayerData().GameGoldTime > PlayerState->GetPlayerData().MaxGameGoldTime) {
						PlayerState->GetPlayerData().GameGoldTime = 0.f;
						PlayerState->GetPlayerData().GameGold++;
					}
				}
			}
		);

		if (TempGameState->GetGameData().GameTimeCount <= 0) {
			TempGameState->GetGameData().bGameOver = true;
		}
		else {
			TempGameState->GetGameData().GameTimeCount -= DeltaSeconds;
		}

		int32 TowersNum = 0;
		TArray<ARuleOfCharacter*>InTowers;
		StoneDefenceUtils::GetAllActor<ATowers>(GetWorld(), InTowers);
		for (ARuleOfCharacter* Temp : InTowers) {
			if (Temp->IsActive()) {
				TowersNum++;
			}
		}
		if (!TowersNum) {
			TempGameState->GetGameData().bGameOver = true;
		}
	}	
	SpawnRuleOfMonster(DeltaSeconds);
	UpdateSkill(DeltaSeconds);
}

void ATD_GameMode::SpawnRuleOfMonster(float DeltaSeconds) {
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {
		if (!TempGameState->GetGameData().bCurrentLevelMissionSuccess)/*当前关卡是否胜利*/ {
			if (!TempGameState->GetGameData().bGameOver) {//游戏是否结束
				if (TempGameState->GetGameData().MobNumberinCurrentStage.Num()) {//是否还有波数
					TempGameState->GetGameData().CurrentSpawnMonsterTime += DeltaSeconds;
					if (TempGameState->GetGameData().bAllowSpawnMonster()) {
						TempGameState->GetGameData().ResetCurrentSpawn();

						int32 MobDifficulty = GetTowerDifficultyParam_Level(GetWorld());
						if (AMonsters* Monster = SpawnMonster(0, MobDifficulty, FVector::ZeroVector, FRotator::ZeroRotator)) {
							TArray<ASpawnPoint*> MonsterSpawnPoints;
							for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
								if (Monster->GetTeamType() == SpawnPoint->Team) {
									MonsterSpawnPoints.Add(SpawnPoint);
								}
							}
							ASpawnPoint* SpawnPoint = MonsterSpawnPoints[FMath::RandRange(0, MonsterSpawnPoints.Num() - 1)];
							Monster->SetActorLocationAndRotation(SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());

							TempGameState->GetGameData().StageDecision();
						}
					}
				}
			}
		}
		else {

		}
	}
}


void ATD_GameMode::SpawnMainTowerRule() {
	for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
		if (SpawnPoint->Team == ETeam::RED) {
			SpawnTower(0,1, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
		}
	}
}


void ATD_GameMode::SpawnTowerRule() {
	for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
		if (SpawnPoint->Team == ETeam::RED) {
			SpawnTower(1, 1, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
		}
	}
}
int32 ATD_GameMode::GetTowerDifficultyParam_Level(UWorld* InWorld) {
	struct FDifficultyParam {
		FDifficultyParam()
			:AverageLevel(0.f), Concentration(0.f), Attack(0.f), Defense(0.f), Variance(0.f) {}

		float AverageLevel;
		float Concentration; //Combination -> Concentration
		float Attack;
		float Defense;
		float Variance; //表示数据离散程度
	};

	auto GetTowerDifficultyParam_Level = [&](TArray<ARuleOfCharacter*> RuleOfCharacter) ->FDifficultyParam {
		int32 Index = 0;
		FDifficultyParam DifficultyParam;
		//计算防御塔平均数值
		for (ARuleOfCharacter* Temp : RuleOfCharacter) {
			if (Temp->IsActive()) {//累加等级、攻击和防御
				DifficultyParam.AverageLevel += (float)Temp->GetCharacterData().Level;
				DifficultyParam.Attack += Temp->GetCharacterData().GetAttack();
				DifficultyParam.Defense += Temp->GetCharacterData().GetArmor();
				Index++;
			}
		}
		// 求得平均等级、攻击和防御
		DifficultyParam.AverageLevel /= Index;
		DifficultyParam.Attack /= Index;
		DifficultyParam.Defense /= Index;


		//计算怪物数值
		for (ARuleOfCharacter* Temp : RuleOfCharacter) {
			if (Temp->IsActive()) {
				DifficultyParam.Variance += FMath::Square((float)Temp->GetCharacterData().Level - DifficultyParam.AverageLevel);
				Index++;
			}
		}
		DifficultyParam.Variance /= Index;

		return DifficultyParam;
	};

	TArray<ARuleOfCharacter*> Towers;
	TArray<ARuleOfCharacter*> Monsters;
	StoneDefenceUtils::GetAllActor<ATowers>(InWorld, Towers);
	StoneDefenceUtils::GetAllActor<AMonsters>(InWorld, Monsters);

	FDifficultyParam TowerDP = GetTowerDifficultyParam_Level(Towers);
	FDifficultyParam MonsterDP = GetTowerDifficultyParam_Level(Monsters);

	int32 ReturnLevel = TowerDP.AverageLevel;
	if (TowerDP.Attack > MonsterDP.Attack) {
		ReturnLevel++;
	}
	if (TowerDP.Defense > MonsterDP.Defense) {
		ReturnLevel++;
	}
	ReturnLevel += FMath::Abs(2 - FMath::Sqrt(TowerDP.Variance / 10.f));
	return ReturnLevel;
}

ARuleOfCharacter* ATD_GameMode::SpawnCharacter(int32 CharacterID,
	int32 CharacterLevel,
	UDataTable* InCharacterData,
	const FVector& Location,
	const FRotator& Rotator) {
	ARuleOfCharacter* InCharacter = nullptr;
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {
		if (InCharacterData) {
			TArray<FCharacterData*> Datas;
			InCharacterData->GetAllRows(TEXT("CharacterData"), Datas);

			auto GetCharacterData = [&](int32 ID) ->FCharacterData* {
				for (auto& Temp : Datas) {
					if (Temp->ID == ID) {
						return Temp;
					}
				}
				return nullptr;
			};

			if (FCharacterData* CharacterData = GetCharacterData(CharacterID)) {
				UClass* NewClass = CharacterData->CharacterBlueprintKey.LoadSynchronous();//负载同步
				if (GetWorld())
					if (ARuleOfCharacter* RuleOfCharacter = GetWorld()->SpawnActor<ARuleOfCharacter>(NewClass, Location, Rotator)) {
						FCharacterData& CharacterDataInstance = TempGameState->AddCharacterData(RuleOfCharacter->GUID, *CharacterData);
						CharacterDataInstance.UpdateHealth();

						//初始化等级
						if (CharacterLevel > 1) {
							for (int32 i = 0; i < CharacterLevel; i++) {
								CharacterDataInstance.UpdateExp(CharacterData->AddEmpiricalValue);
							}
						}

						TempGameState->InitSkill(CharacterDataInstance);
						RuleOfCharacter->RegisterTeam();
						InCharacter = RuleOfCharacter;
					}
			}
		}
	}

	return InCharacter;
}

ATowers* ATD_GameMode::SpawnTower(int32 CharacterID,
	int32 CharacterLevel,
	const FVector& Location,
	const FRotator& Rotator) {
	return SpawnCharacter<ATowers>(CharacterID, CharacterLevel, GetGameState<ATD_GameState>()->AITowerCharacterData, Location, Rotator);
}

AMonsters* ATD_GameMode::SpawnMonster(int32 CharacterID,
	int32 CharacterLevel,
	const FVector& Location,
	const FRotator& Rotator) {
	return SpawnCharacter<AMonsters>(CharacterID, CharacterLevel, GetGameState<ATD_GameState>()->AIMonsterCharacterData, Location, Rotator);
}

void ATD_GameMode::UpdateSkill(float DeltaSeconds) {
	if (ATD_GameState* NewGameState = GetGameState<ATD_GameState>()) {
		//GetTeam() -> GetSkillTaker()
		auto GetSkillTaker = [&](TArray<TPair<FGuid, FCharacterData>*>& TeamArray, TPair<FGuid, FCharacterData>& InOwner, float AttackRange, bool bAllies = false) {
			auto TeamIner = [&](TArray<TPair<FGuid, FCharacterData>*>& TeamArray, TPair<FGuid, FCharacterData>& Target, float AttackRange) {
				if (AttackRange != 0) {
					float Distance = (Target.Value.Location - InOwner.Value.Location).Size();
					if (Distance <= AttackRange) {
						//将范围内的单位纳入技能承受队列
						TeamArray.Add(&Target);
					}
				}
				else {//若距离为零，则无视距离
					TeamArray.Add(&Target);
				}
			};
			
			for (auto& Temp : NewGameState->GetSaveData()->CharacterDatas) {
				if (bAllies) {
					if (Temp.Value.Team == InOwner.Value.Team) {//此处InOwner和Temp的关系见下方FindMostClosedTargetInRange中的解释
						TeamIner(TeamArray, Temp, AttackRange);
					}
				}
				else {
					if (Temp.Value.Team != InOwner.Value.Team) {
						TeamIner(TeamArray, Temp, AttackRange);
					}
				}
			}
		};

		auto IsVerificationSkill = [](FCharacterData& SkillList, int32 SkillID)-> bool {//此处SkillList前的const由于在AddSkill表达式中，TMap.Add()不能传入Const值，故删掉
			for (auto& AdditionalSkill : SkillList.AdditionalSkillData) {
				if (AdditionalSkill.Value.SkillID == SkillID) {
					return true;
				}
			}
			return false;
		};
		//为单个角色添加技能
		auto AddSkill = [&](TPair<FGuid, FCharacterData>& SkillTakerData, FSkillData& InSkill) {
			if (!IsVerificationSkill(SkillTakerData.Value, InSkill.SkillID)) {
				FGuid TempSkillID = FGuid::NewGuid();	

				SkillTakerData.Value.AdditionalSkillData.Add(TempSkillID, InSkill);

				//通知代理 在UI模块显示相应技能图标	
				CallUpdateAllClient([&](ATD_PlayerController* MyPlayerController) {
					MyPlayerController->AddSkillSlot_Client(TempSkillID);
					}
				);
			}
		};
		//为多个角色添加技能
		auto AddSkills = [&](TArray<TPair<FGuid, FCharacterData>*>& SkillTakerDataArray, FSkillData& InSkill) {
			for (auto& Data : SkillTakerDataArray) {
				AddSkill(*Data, InSkill);
			}
		};

		auto FindMostClosedTargetInRange = [&](TPair<FGuid, FCharacterData>& InOwner, bool bAllies = false) -> TPair<FGuid, FCharacterData>* {
			float TargetDistance = 9999999999;
			FGuid Index;

			auto InitMostCloseTarget = [&](TPair<FGuid, FCharacterData>& Pair) {
				FVector Location = Pair.Value.Location;
				FVector TempVector = Location - InOwner.Value.Location;
				float Distance = TempVector.Size();

				if (Distance < TargetDistance && Pair.Value.Health > 0) {
					Index = Pair.Key;
					TargetDistance = Distance;
				}
			};

			for (auto& Temp : NewGameState->GetSaveData()->CharacterDatas) {
				if (InOwner.Key != Temp.Key) {//排除自己
					if (bAllies) {
						//此处的InOwner是主体，Temp是客体。 InOwner在第一层遍历中被选中，然后在第二层遍历中和每一个其他客体进行队伍比较
						//寻敌
						if (InOwner.Value.Team != Temp.Value.Team) {
							InitMostCloseTarget(Temp);
						}
					}
					else {
						//寻找友军
						if (InOwner.Value.Team == Temp.Value.Team) {
							InitMostCloseTarget(Temp);
						}
					}
				}
			}

			if (Index != FGuid()) 
				for (TPair<FGuid, FCharacterData> GameTemp : NewGameState->GetSaveData()->CharacterDatas) 
					if (GameTemp.Key == Index)
						return &GameTemp;
			return nullptr;
		};

		//获取技能队列
		const TArray<FSkillData*>& SkillDataTemplate = NewGameState->GetSkillDataFromTable();
		//获取所有技能
		for (auto& Temp : NewGameState->GetSaveData()->CharacterDatas) {
			//计算与更新清除列表
			TArray<FGuid> RemoveSkillArray;
			for (auto& SkillTemp : Temp.Value.AdditionalSkillData) {
				if (SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::BURST)
					RemoveSkillArray.Add(SkillTemp.Key);
				//若为
				if (SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::SECTION ||
					SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::ITERATION) {
					SkillTemp.Value.SkillDuration += DeltaSeconds;
					if (SkillTemp.Value.SkillDuration >= SkillTemp.Value.MaxSkillDuration) {
						RemoveSkillArray.Add(SkillTemp.Key);
					}
				}
				//若为持续性技能，每X秒执行一次增减益
				if (SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::ITERATION) {
					SkillTemp.Value.SkillDuration += DeltaSeconds;
					if (SkillTemp.Value.SkillDuration >= 1.0f) {
						SkillTemp.Value.SkillDuration = 0.f;
						//若为增益
						if (SkillTemp.Value.SkillType.SkillBoostType == ESkillBoostType::ADD) {
							Temp.Value.Health += SkillTemp.Value.HealthModify;
							Temp.Value.PhysicalAttack += SkillTemp.Value.PhysicalAttackModify;
							Temp.Value.Armor += SkillTemp.Value.ArmorModify;
							Temp.Value.AttackSpeed += SkillTemp.Value.AttackSpeedModify;
							Temp.Value.Gold += SkillTemp.Value.GoldModify;

						}
						else {//若为减益
							Temp.Value.Health -= SkillTemp.Value.HealthModify;
							Temp.Value.PhysicalAttack -= SkillTemp.Value.PhysicalAttackModify;
							Temp.Value.Armor -= SkillTemp.Value.ArmorModify;
							Temp.Value.AttackSpeed -= SkillTemp.Value.AttackSpeedModify;
							Temp.Value.Gold -= SkillTemp.Value.GoldModify;
						}
					}
				}
				//通知客户端渲染Projectile
				CallUpdateAllClient([&](ATD_PlayerController* MyPlayerController) {
					MyPlayerController->Spawn_Projectile_Client(Temp.Key, SkillTemp.Value.ProjectileClass);
					});
			}

			//清除
			for (FGuid& RemoveID : RemoveSkillArray)
				Temp.Value.AdditionalSkillData.Remove(RemoveID);

			//更新每一个技能的CD
			for (auto& InSkill : Temp.Value.CharacterSkill) {
				InSkill.CDTime += DeltaSeconds;
				if (InSkill.CDTime >= Temp.Value.CD) {
					InSkill.CDTime = 0.f;
					//判断该技能是群体 或单体攻击					
					if (InSkill.SkillType.SkillTargetNumType == ESkillTargetNumType::MULTIPLE) {
						TArray<TPair<FGuid, FCharacterData>*> SkillTakerDataArray;
						if (InSkill.SkillType.SkillTargetType == ESkillTargetType::ALLIES)
							GetSkillTaker(SkillTakerDataArray, Temp, InSkill.AttackRange, true);
						else if (InSkill.SkillType.SkillTargetType == ESkillTargetType::ENEMY)
							GetSkillTaker(SkillTakerDataArray, Temp, InSkill.AttackRange, false);
						AddSkills(SkillTakerDataArray, InSkill);
					}
					else if (InSkill.SkillType.SkillTargetNumType == ESkillTargetNumType::SINGLE) {
						TPair<FGuid, FCharacterData>* MostCloseTarget = nullptr;
						if (InSkill.SkillType.SkillTargetType == ESkillTargetType::ALLIES) {
							MostCloseTarget = FindMostClosedTargetInRange(Temp);
						}
						else if (InSkill.SkillType.SkillTargetType == ESkillTargetType::ENEMY) {
							MostCloseTarget = FindMostClosedTargetInRange(Temp, true);
						}
						if (MostCloseTarget) {
							AddSkill(*MostCloseTarget, InSkill);
						}
					}
				}
			}
		}
	}
}


void ATD_GameMode::CallUpdateAllClient(TFunction<void(ATD_PlayerController* MyPlayerController)> InImplement) {
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		if (ATD_PlayerController* MyPlayerController = Cast<ATD_PlayerController>(It->Get())) {
			InImplement(MyPlayerController);
		}
	}
}