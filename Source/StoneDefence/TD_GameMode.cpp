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
#include "Core/GameCore/Synty_Camera.h"
#include "Core/GameCore/TD_GameInstance.h"




ATD_GameMode::ATD_GameMode()
{
	//将UE中的pawn，	contorller，PlayerState等数据与C++代码绑定起来
	GameStateClass = ATD_GameState::StaticClass();
	PlayerControllerClass = ATD_PlayerController::StaticClass();
	DefaultPawnClass = ASynty_Camera::StaticClass();
	PlayerStateClass = ATD_PlayerState::StaticClass();
	HUDClass = ARuleOfHUD::StaticClass();
}

void ATD_GameMode::BeginPlay() {
	Super::BeginPlay();

	// 查找场景中的自定义摄像机
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASynty_Camera::StaticClass(), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		ASynty_Camera* Camera = Cast<ASynty_Camera>(FoundCameras[0]);
		if (Camera && GetWorld()->GetFirstPlayerController())
		{
			// 设置这个摄像机为视图目标
			GetWorld()->GetFirstPlayerController()->SetViewTarget(Camera);
		}
	}
}

void ATD_GameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	UpdatePlayerData(DeltaSeconds);
	UpdateGameData(DeltaSeconds);
	UpdateMonsterSpawnRule(DeltaSeconds);
	UpdateSkill(DeltaSeconds);
	UpdateInventory(DeltaSeconds);
	UpdatePlayerSkill(DeltaSeconds);
}


void ATD_GameMode::InitDataFormArchives()
{
	if (ATD_GameState* InGameState = GetGameState<ATD_GameState>())
	{
		//初始化游戏数据
		InGameState->GetSaveData();

		//初始化存储的列表
		InGameState->GetGameSaveSlotList();

		//初始化玩家数据表
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController)
			{
				if (ATD_PlayerState* InPlayerState = MyPlayerController->GetPlayerState<ATD_PlayerState>())
				{
					InPlayerState->GetSaveData();//初始化玩家数据表
				}
			});
	}
}

void ATD_GameMode::InitStandardData()
{
	if (ATD_GameState* InGameState = GetGameState<ATD_GameState>())
	{
		InGameState->GetGameData().AssignedMonsterAmount();
		//生成主塔
		SpawnMainTowerRule();
	}
}

void ATD_GameMode::UpdateMonsterSpawnRule(float DeltaSeconds) {
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {
		if (!TempGameState->GetGameData().bCurrentLevelMissionSuccess)/*当前关卡是否胜利*/ {
			if (!TempGameState->GetGameData().bGameOver) {//游戏是否结束
				if (TempGameState->GetGameData().MobNumberinCurrentStage.Num()) {//是否还有波数
					TempGameState->GetGameData().CurrentSpawnMonsterTime += DeltaSeconds;
					if (TempGameState->GetGameData().bAllowSpawnMonster()) {
						TempGameState->GetGameData().ResetCurrentSpawn();

						int32 MobDifficulty = GetTowerDifficultyParam_Level(GetWorld());
						if (AMonsters* Monster = SpawnMonster(0, FVector::ZeroVector, FRotator::ZeroRotator, MobDifficulty)) {
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
			/*播放胜利画面*/
		}
	}
}


void ATD_GameMode::SpawnMainTowerRule() {
	for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
		if (SpawnPoint->Team == ETeam::RED) {
			SpawnTower(0, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), 1);
		}
	}
}


void ATD_GameMode::SpawnTowerRule() {
	for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld())) {
		if (SpawnPoint->Team == ETeam::RED) {
			SpawnTower(1, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), 1);
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
	UDataTable* InCharacterData,
	const FVector& Location,
	const FRotator& Rotator,
	int32 CharacterLevel
	/*const FGuid& InCharacterGuid*/) {
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
						//RuleOfCharacter->ResetGUID();
						FCharacterData& CharacterDataInstance = TempGameState->AddCharacterData(RuleOfCharacter->GUID, *CharacterData);
						CharacterDataInstance.UpdateHealth();

						//初始化等级
						if (CharacterLevel > 1) {
							for (int32 i = 0; i < CharacterLevel; i++) {
								CharacterDataInstance.UpdateExp(CharacterData->AddEmpiricalValue);
							}
						}
						//初始话防御塔被动技能
						RuleOfCharacter->InitPassiveSkill();
						RuleOfCharacter->RegisterTeam();
						InCharacter = RuleOfCharacter;
					}
			}
		}
	}

	return InCharacter;
}

ATowers* ATD_GameMode::SpawnTower(int32 CharacterID,	
	const FVector& Location,
	const FRotator& Rotator,
	int32 CharacterLevel
	/*const FGuid& InCharacterGuid*/) {
	return SpawnCharacter<ATowers>(CharacterID, GetGameState<ATD_GameState>()->AITowerCharacterData, Location, Rotator, CharacterLevel/*, InCharacterGuid*/);
}

AMonsters* ATD_GameMode::SpawnMonster(int32 CharacterID,
	const FVector& Loction,
	const FRotator& Rotator,
	int32 CharacterLevel
	/*const FGuid& InCharacterGuid*/){
	return SpawnCharacter<AMonsters>(CharacterID, GetGameState<ATD_GameState>()->AIMonsterCharacterData, Loction, Rotator, CharacterLevel/*, InCharacterGuid*/);
}

void ATD_GameMode::UpdateSkill(float DeltaSeconds) {
	if (ATD_GameState* NewGameState = GetGameState<ATD_GameState>()) {
		////GetTeam() -> GetSkillTaker()
		//auto GetSkillTaker = [&](TArray<TPair<FGuid, FCharacterData>*>& TeamArray, TPair<FGuid, FCharacterData>& InOwner, float AttackRange, bool bAllies = false) {
		//	auto TeamIner = [&](TArray<TPair<FGuid, FCharacterData>*>& TeamArray, TPair<FGuid, FCharacterData>& Target, float AttackRange) {
		//		if (AttackRange != 0) {
		//			float Distance = (Target.Value.Location - InOwner.Value.Location).Size();
		//			if (Distance <= AttackRange) {
		//				//将范围内的单位纳入技能承受队列
		//				TeamArray.Add(&Target);
		//			}
		//		}
		//		else {//若距离为零，则无视距离
		//			TeamArray.Add(&Target);
		//		}
		//	};
		//	
		//	for (auto& Temp : NewGameState->GetSaveData()->CharacterDatas) {
		//		if (bAllies) {
		//			if (Temp.Value.Team == InOwner.Value.Team) {//此处InOwner和Temp的关系见下方FindMostClosedTargetInRange中的解释
		//				TeamIner(TeamArray, Temp, AttackRange);
		//			}
		//		}
		//		else {
		//			if (Temp.Value.Team != InOwner.Value.Team) {
		//				TeamIner(TeamArray, Temp, AttackRange);
		//			}
		//		}
		//	}
		//};


		//auto FindMostClosedTargetInRange = [&](TPair<FGuid, FCharacterData>& InOwner, bool bAllies = false) -> TPair<FGuid, FCharacterData>* {
		//	float TargetDistance = 9999999999;
		//	FGuid Index;

		//	auto InitMostCloseTarget = [&](TPair<FGuid, FCharacterData>& Pair) {
		//		FVector Location = Pair.Value.Location;
		//		FVector TempVector = Location - InOwner.Value.Location;
		//		float Distance = TempVector.Size();

		//		if (Distance < TargetDistance && Pair.Value.Health > 0) {
		//			Index = Pair.Key;
		//			TargetDistance = Distance;
		//		}
		//	};

		//	for (auto& Temp : NewGameState->GetSaveData()->CharacterDatas) {
		//		if (InOwner.Key != Temp.Key) {//排除自己
		//			if (bAllies) {
		//				//此处的InOwner是主体，Temp是客体。 InOwner在第一层遍历中被选中，然后在第二层遍历中和每一个其他客体进行队伍比较
		//				//寻敌
		//				if (InOwner.Value.Team != Temp.Value.Team) {
		//					InitMostCloseTarget(Temp);
		//				}
		//			}
		//			else {
		//				//寻找友军
		//				if (InOwner.Value.Team == Temp.Value.Team) {
		//					InitMostCloseTarget(Temp);
		//				}
		//			}
		//		}
		//	}

		//	if (Index != FGuid()) 
		//		for (TPair<FGuid, FCharacterData>& GameTemp : NewGameState->GetSaveData()->CharacterDatas) 
		//			if (GameTemp.Key == Index)
		//				return &GameTemp;
		//	return nullptr;
		//};

		////获取技能队列
		//const TArray<FSkillData*>& SkillDataTemplate = NewGameState->GetSkillDataFromTable();



		//获取所有角色的所有技能
		for (auto& Temp : NewGameState->GetSaveData()->CharacterDatas) {
			if (Temp.Value.Health > 0.f) {
				//计算与更新清除列表
				TArray<FGuid> RemoveSkillArray;//存储爆发类，作用时间结束的持续类技能
				for (auto& SkillTemp : Temp.Value.AdditionalSkillData) {
					SkillTemp.Value.SkillDuration -= DeltaSeconds;
					if (SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::BURST)
						RemoveSkillArray.Add(SkillTemp.Key);
					//若为
					if (SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::SECTION ||
						SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::ITERATION) {
						SkillTemp.Value.SkillDuration += DeltaSeconds;
						if (SkillTemp.Value.SkillDuration <= 0.f) {
							RemoveSkillArray.Add(SkillTemp.Key);
						}
					}
					//若为持续性技能，每秒执行一次增减益
					if (SkillTemp.Value.SkillType.SkillTimeType == ESkillTimeType::ITERATION) {

						if (SkillTemp.Value.SkillDuration <= 0.f) {
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

							//通知客户端渲染Projectile,之所以通过子弹渲染是因为会让子弹停止运动，然后播放效果，相当于播放特效
							StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
								MyPlayerController->Spawn_Projectile_Client(Temp.Key, SkillTemp.Value.ID);
							});
						}
					}
					
				}

				//清除爆发类，作用时间结束的持续类技能
				for (FGuid& RemoveID : RemoveSkillArray) {
					//通知客户端移除技能图标
					StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
						MyPlayerController->RemoveSkillSlot_Server(Temp.Key, RemoveID);
						});
					Temp.Value.AdditionalSkillData.Remove(RemoveID);

				}

				TArray<FSkillData> RemoveRepeatedSkillArray;//存储重复施加的持续性技能
				//更新每一个技能的CD
				for (auto& InSkill : Temp.Value.CharacterSkill) {
					InSkill.SkillCD -= DeltaSeconds;
					if (InSkill.SkillCD <= 0.f) {
						InSkill.SkillCD = InSkill.MaxSkillCD;

						//为防止同种技能叠加，此处过滤掉已生效的技能
						if (!InSkill.bSkillEffected) {//若技能未触发过，触发						
							//判断该技能是群体 或单体攻击					
							InSkill.bSkillEffected = true;
						}
						else {
							//若技能释放重复，重置CD , 并将该技能加入重复移除队列
							InSkill.ResetCD();
							RemoveRepeatedSkillArray.Add(InSkill);
						}
					}
				}

				//移除重复施加的持续性技能
				for (FSkillData& RepeatedSkill : RemoveRepeatedSkillArray){
					Temp.Value.CharacterSkill.Remove(RepeatedSkill);
					if (RepeatedSkill.SubmissionSkillRequestType == ESubmissionSkillRequestType::AUTO) {
						StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
							MyPlayerController->Spawn_Projectile_Client(Temp.Key, RepeatedSkill.ID);
						});  
					}
					else if(RepeatedSkill.SubmissionSkillRequestType == ESubmissionSkillRequestType::MANUAL){
						//手动不进行更新
					}
					
					////通知客户端移除技能图标
					//StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
					//	MyPlayerController->RemoveSkillSlot_Server(Temp.Key, RepeatedSkill.SkillID);
					//	});
					//Temp.Value.AdditionalSkillData.Remove(RepeatedSkill.SkillID);
				}
			}
		}
	}
}

void ATD_GameMode::UpdateInventory(float DeltaSeconds) {
	ATD_GameState* InGameState = GetGameState<ATD_GameState>();
	if (!InGameState) return;

	StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController)
		{
			ATD_PlayerState* InPlayerState = MyPlayerController->GetPlayerState<ATD_PlayerState>();
			if (!InPlayerState) return;

			for (auto& Tmp : InPlayerState->GetSaveData()->BuildingTowers)
			{
				if (!Tmp.Value.IsValid()) return;
				FBuildingTowers& BTData = Tmp.Value;
				if (BTData.isCDFreezed) return;
				if (BTData.isIconDragged) return;

				if (BTData.CurrentConstructionTowersCD > 0)
				{
					BTData.CurrentConstructionTowersCD -= DeltaSeconds;
					BTData.CallUpdateTowrsInfoOrNot = true;

					//通知客户端更新我们的装备CD
					StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController)
						{
							//最终链式调用Inventory中的UpdateInventorySlot
							MyPlayerController->UpdateInventory_Client(Tmp.Key, true);
						});
				}
				else if (BTData.CallUpdateTowrsInfoOrNot)
				{
					BTData.CallUpdateTowrsInfoOrNot = false;
					//准备构建的塔
					BTData.TowersPrepareBuildingNumber--;


					//CD结束直接生成对应单位
					TArray<ASpawnPoint*>MarineSpawnPoints;
					for (ASpawnPoint* SpawnPoint : StoneDefenceUtils::GetAllActor<ASpawnPoint>(GetWorld()))
						if (ETeam::BLUE == SpawnPoint->Team)
							MarineSpawnPoints.Add(SpawnPoint);

					AMarine* Marine = SpawnMarine(-1, FVector::ZeroVector, FRotator::ZeroRotator, 1);
					if (Marine) {
						ASpawnPoint* OneOfSpawnPoint = MarineSpawnPoints[FMath::RandRange(0, MarineSpawnPoints.Num() - 1)];
						Marine->SetActorLocationAndRotation(OneOfSpawnPoint->GetActorLocation(), OneOfSpawnPoint->GetActorRotation());
					}


					//由于直接生成，暂时禁用
					//Tmp.Value.TowersConstructionNumber++;

					//通知客户端更新我们的装备CD
					StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController)
						{
							MyPlayerController->UpdateInventory_Client(Tmp.Key, false);
						});

					if (BTData.TowersPrepareBuildingNumber > 0)
					{
						BTData.ResetCD();
					}
				}

			}
		}
	);//CallUpdateAllClient()
}


void ATD_GameMode::UpdatePlayerData(float DeltaSeconds) {

	//更新游戏金币
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {

		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
			if (ATD_PlayerState* PlayerState = MyPlayerController->GetPlayerState<ATD_PlayerState>()) {
				if (TempGameState->GetGameData().bTimeFreezed == false) {
					//每1.25秒+1金币
					PlayerState->GetPlayerData().GameGoldTime += DeltaSeconds;
					if (PlayerState->GetPlayerData().GameGoldTime > PlayerState->GetPlayerData().MaxGameGoldTime) {
						PlayerState->GetPlayerData().GameGoldTime = 0.f;
						PlayerState->GetPlayerData().GameGold++;
					}
				}				
			}
		});
	}
}

void ATD_GameMode::UpdateGameData(float DeltaSeconds) {
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {
		//游戏倒计时，更新游戏结束状态
		if (TempGameState->GetGameData().GameTimeCount <= 0) {
			TempGameState->GetGameData().bGameOver = true;
		}
		else if(TempGameState->GetGameData().bTimeFreezed == false){
			TempGameState->GetGameData().GameTimeCount -= DeltaSeconds;
		}

		//以下部分可以改为在开局初始化塔的数量，并在塔被摧毁，建造塔的时候进行数值更改，避免Tick
		int32 TowersNum = 0;
		TArray<ARuleOfCharacter*>InTowers;
		StoneDefenceUtils::GetAllActor<ATowers>(GetWorld(), InTowers);
		for (ARuleOfCharacter* Temp : InTowers) {
			if (Temp->IsActive()) {
				TowersNum++;
			}
		}
		if (!TowersNum) {
			//TempGameState->GetGameData().bGameOver = true;
		}
	}

}

void ATD_GameMode::UpdatePlayerSkill(float DeltaSeconds) {
	if (ATD_GameState* TempGameState = GetGameState<ATD_GameState>()) {
		StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
			if (ATD_PlayerState* PlayerState = MyPlayerController->GetPlayerState<ATD_PlayerState>()) {
				for (auto& Temp : PlayerState->GetSaveData()->PlayerSkillDatas) {
					if (Temp.Value.IsValid()) {
						if (Temp.Value.SkillCD > 0.f && TempGameState->GetGameData().bTimeFreezed == false) {
							Temp.Value.SkillCD -= DeltaSeconds;
							Temp.Value.bSkillEffected = true;
							StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
								MyPlayerController->UpdatePlayerSkill_Client(Temp.Key, true);
							});
						}
						else if (Temp.Value.bSkillEffected) {
							Temp.Value.bSkillEffected = false;
							StoneDefenceUtils::CallUpdateAllClient(GetWorld(), [&](ATD_PlayerController* MyPlayerController) {
								MyPlayerController->UpdatePlayerSkill_Client(Temp.Key, true);
							});
						}
					}
				}
			}
		});
	}
}

