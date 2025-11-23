// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SteeringBehavior/SteeringBehaviorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Character/Core/RuleOfCharacter.h"
#include "NavigationSystem.h" 
#include "NavigationPath.h"


// Sets default values for this component's properties
USteeringBehaviorComponent::USteeringBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    LastSteeringForce = FVector::ZeroVector;
}


// Called when the game starts
void USteeringBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USteeringBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



FVector USteeringBehaviorComponent::CalculateSteeringForce(FVector TargetLocation)
{
    // 1. 分别计算各种行为力
    FVector SeekForce = (!TargetLocation.IsZero()) ? Seek(TargetLocation).GetSafeNormal() * SeekWeight : FVector::ZeroVector;
    FVector PathFollowForce = (PathPoints.Num() > 0) ? FollowPath().GetSafeNormal() * PathFollowWeight : FVector::ZeroVector;
    FVector AvoidForce = AvoidObstacles().GetSafeNormal() * AvoidanceWeight;
    FVector SeparateForce = Separation().GetSafeNormal() * SeparationWeight;

    // 2. 行为优先级融合
    FVector TotalForce = FVector::ZeroVector;

    // 避障力最高优先级
    if (AvoidForce.SizeSquared() > 0.1f)
    {
        TotalForce = AvoidForce * AvoidancePriorityWeight;
        // 混合主要目标力，避免完全偏离
        TotalForce += (SeekForce + PathFollowForce).GetSafeNormal() * SeekWeight * (1 - AvoidancePriorityWeight);
    }
    // 分离力次优先级
    else if (SeparateForce.SizeSquared() > 0.1f)
    {
        TotalForce = SeparateForce * SeparationPriorityWeight;
        TotalForce += (SeekForce + PathFollowForce).GetSafeNormal() * SeekWeight * (1 - SeparationPriorityWeight);
    }
    // 常规寻路
    else
    {
        TotalForce = SeekForce + PathFollowForce;
    }

    // 3. 应用平滑滤波（惯性）
    if (LastSteeringForce.IsZero())
    {
        LastSteeringForce = TotalForce; // 初始化
    }
    else
    {
        TotalForce = FMath::Lerp(LastSteeringForce, TotalForce, SmoothFactor);
    }

    // 保存当前力用于下一帧
    LastSteeringForce = TotalForce;

    // 4. 死区过滤：忽略微小的力
    if (TotalForce.SizeSquared() < 0.01f)
    {
        LastSteeringForce = FVector::ZeroVector; // 重置
        return FVector::ZeroVector;
    }

    // 5. 返回最终的带速度的力向量
    return TotalForce.GetSafeNormal() * MoveSpeed;
}



FVector USteeringBehaviorComponent::Seek(const FVector& TargetLocaiton)
{
    AActor* Owner = GetOwnerActor();
    if (!Owner) return FVector::ZeroVector;
    return (TargetLocaiton - Owner->GetActorLocation()).GetSafeNormal();
}



FVector USteeringBehaviorComponent::AvoidObstacles()
{
    AActor* Owner = GetOwnerActor();
    if (!Owner) return FVector::ZeroVector;

    FVector ActorLocation = Owner->GetActorLocation();
    FRotator ActorRotation = Owner->GetActorRotation();

    // 定义三个探测方向：左前(45°)、正前(0°)、右前(-45°)
    TArray<FRotator> DetectionRotations = {
        ActorRotation + FRotator(0, 45, 0),   // 左前45度
        ActorRotation,                        // 正前
        ActorRotation + FRotator(0, -45, 0)   // 右前45度
    };

    // 存储每个方向最近的障碍物位置
    TArray<FVector> ObstaclePositions;

    // 对每个方向发射射线
    for (const FRotator& DetectionRotation : DetectionRotations)
    {
        FVector Direction = DetectionRotation.Vector();
        FVector Start = ActorLocation;
        FVector End = Start + Direction * AvoidanceRadius;

        // 射线检测参数
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

        TArray<AActor*> IgnoreActors;
        IgnoreActors.Add(Owner);

        FHitResult HitResult;
        bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
            GetWorld(),
            Start,
            End,
            ObjectTypes,
            false,
            IgnoreActors,
            EDrawDebugTrace::ForDuration, // 调试时显示射线
            HitResult,
            true,
            FLinearColor::Red,
            FLinearColor::Green,
            0.1f
        );

        // 如果检测到障碍物，存储最近的碰撞点
        if (bHit)
        {
            ObstaclePositions.Add(HitResult.ImpactPoint);

            // 调试绘制：在障碍物位置画红色球体
            DrawDebugSphere(
                GetWorld(),
                HitResult.ImpactPoint,
                10.0f,
                12,
                FColor::Red,
                false,
                0.5f
            );
        }
    }

    // 如果没有检测到任何障碍物，返回零向量
    if (ObstaclePositions.Num() == 0)
    {
        return FVector::ZeroVector;
    }

    // 基于障碍物位置计算躲避力
    return CalculateAvoidanceForce(ActorLocation, ActorRotation, ObstaclePositions);
}



FVector USteeringBehaviorComponent::CalculateAvoidanceForce(const FVector& ActorLocation, const FRotator& ActorRotation, const TArray<FVector>& ObstaclePositions)
{
    FVector AvoidanceForce = FVector::ZeroVector;
    FVector ForwardDir = ActorRotation.Vector();
    FVector RightDir = ActorRotation.RotateVector(FVector::RightVector);

    for (const FVector& ObstaclePos : ObstaclePositions)
    {
        // 计算从角色到障碍物的向量
        FVector ToObstacle = ObstaclePos - ActorLocation;
        float Distance = ToObstacle.Size();

        // 将障碍物向量投影到角色的前/右坐标系
        float ForwardDot = FVector::DotProduct(ToObstacle.GetSafeNormal(), ForwardDir);
        float RightDot = FVector::DotProduct(ToObstacle.GetSafeNormal(), RightDir);

        // 躲避力强度与距离成反比（越近的障碍物权重越高）
        float ForceStrength = UKismetMathLibrary::MapRangeClamped(Distance, 0.0f, AvoidanceRadius, MaxAvoidForce, 0.0f);

        // 计算躲避方向：远离障碍物（主要是左右方向）
        FVector AvoidDir = FVector(0, -RightDot, 0); // 仅在水平方向躲避

        // 累加躲避力
        AvoidanceForce += AvoidDir * ForceStrength;
    }

    // 将躲避力转换为世界空间向量
    AvoidanceForce = ActorRotation.RotateVector(AvoidanceForce);

    // 调试绘制：显示躲避力方向
    DrawDebugLine(
        GetWorld(),
        ActorLocation,
        ActorLocation + AvoidanceForce * 3,
        FColor::Green,
        false,
        0.5f,
        0,
        2.0f
    );

    return AvoidanceForce;
}



FVector USteeringBehaviorComponent::FollowPath()
{
    AActor* Owner = GetOwnerActor();
    if (!Owner || PathPoints.Num() == 0 || CurrentWaypointIndex >= PathPoints.Num())
    {
        return FVector::ZeroVector;
    }

    FVector CurrentWaypoint = PathPoints[CurrentWaypointIndex];
    float Distance = FVector::Distance(Owner->GetActorLocation(), CurrentWaypoint);

    if (Distance < 100.0f)
    {
        CurrentWaypointIndex++;
        //若已经是最后一个目标点
        if (CurrentWaypointIndex >= PathPoints.Num())
        {
            PathPoints.Empty();
            return FVector::ZeroVector;
        }
        //若有则跟随下一目标
        CurrentWaypoint = PathPoints[CurrentWaypointIndex];
    }

    return (CurrentWaypoint - Owner->GetActorLocation()).GetSafeNormal();
}



FVector USteeringBehaviorComponent::Separation()
{
    AActor* Owner = GetOwnerActor();
    if (!Owner || !GetWorld()) return FVector::ZeroVector;

    FVector SeparationForce = FVector::ZeroVector;
    FVector OwnerLocation = Owner->GetActorLocation();
    int32 NeighborCount = 0;

    // 设置检测参数
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(Owner);

    // 检测半径内的其他单位
    TArray<AActor*> Neighbors;
    // 设置对象类型查询
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 检测Pawn类型

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        OwnerLocation,
        SeparationRadius,
        ObjectTypes, // 使用对象类型查询数组
        SeparationActorClass ? SeparationActorClass : ARuleOfCharacter::StaticClass(),
        IgnoredActors,
        Neighbors
    );

    // 计算分离力
    for (AActor* Neighbor : Neighbors)
    {
        if (!Neighbor) continue;

        FVector NeighborLocation = Neighbor->GetActorLocation();
        FVector ToOwner = OwnerLocation - NeighborLocation;
        float Distance = ToOwner.Size();

        // 距离为0时跳过（避免除以0）
        if (Distance < 1.0f) continue;

        // 分离力与距离成反比（越近的单位排斥力越强）
        FVector Direction = ToOwner.GetSafeNormal();
        float Strength = UKismetMathLibrary::MapRangeClamped(Distance, 0.0f, SeparationRadius, 1.0f, 0.0f);

        SeparationForce += Direction * Strength;
        NeighborCount++;

        // 调试绘制分离向量
        DrawDebugLine(
            GetWorld(),
            OwnerLocation,
            OwnerLocation + Direction * 100,
            FColor::Purple,
            false,
            0.1f,
            0,
            2.0f
        );
    }

    // 如果有邻居，返回平均分离力
    if (NeighborCount > 0)
    {
        SeparationForce /= NeighborCount;
        return SeparationForce.GetSafeNormal();
    }

    return FVector::ZeroVector;
}



bool USteeringBehaviorComponent::CalculatePathToLocation(const FVector& TargetLocation)
{
    AActor* Owner = GetOwnerActor();
    if (!Owner || !GetWorld()) return false;

    // 获取导航系统
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return false;

    FNavLocation NavStart, NavEnd;

    // 将起点和终点投影到NavMesh上
    if (!NavSys->ProjectPointToNavigation(Owner->GetActorLocation(), NavStart) ||
        !NavSys->ProjectPointToNavigation(TargetLocation, NavEnd))
    {
        return false; // 无法找到有效导航点
    }

    // 计算路径
    UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(
        GetWorld(),
        NavStart.Location,
        NavEnd.Location,
        Owner // 可选：指定寻路的Actor
    );

    if (Path && Path->IsValid())
    {
        PathPoints = Path->PathPoints;
        CurrentWaypointIndex = 0;

        // 调试绘制路径
        for (int32 i = 0; i < PathPoints.Num() - 1; i++)
        {
            DrawDebugLine(
                GetWorld(),
                PathPoints[i],
                PathPoints[i + 1],
                FColor::Blue,
                false,
                5.0f,
                0,
                2.0f
            );
        }

        return true;
    }

    return false;
}

