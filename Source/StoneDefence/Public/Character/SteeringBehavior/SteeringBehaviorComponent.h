// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteeringBehaviorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEDEFENCE_API USteeringBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    // 移动参数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
        float MoveSpeed = 200.0f;



    // 行为权重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Weight")
        float SeekWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Weight")
        float AvoidanceWeight = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Weight")
        float PathFollowWeight = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Weight")
        float SeparationWeight = 2.5f; // 分离行为权重（通常比寻路高）

    UPROPERTY(EditAnywhere, Category = "Steering|Priority")
        float AvoidancePriorityWeight = 0.8f; // 避障优先级权重

    UPROPERTY(EditAnywhere, Category = "Steering|Priority")
        float SeparationPriorityWeight = 0.6f; // 分离优先级权重

    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|ObstacleAvoidForce")
        float AvoidanceRadius = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|ObstacleAvoidForce")
        float MaxAvoidForce = 300.0f;



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Separation")
        float SeparationRadius = 150.0f; // 分离检测半径

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Separation")
        TSubclassOf<AActor> SeparationActorClass; // 需要分离的Actor类型


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering|Damping")
        float SmoothFactor = 0.2f; // 平滑因子（0-1，越小越平滑）

private:

    // 目标和路径数据
    TArray<FVector> PathPoints;
    int32 CurrentWaypointIndex = 0;

    FVector LastSteeringForce; // 上一帧的转向力
    



public:	
	// Sets default values for this component's properties
	USteeringBehaviorComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
    // 计算最终转向力
    UFUNCTION(BlueprintCallable, Category = "Steering")
        FVector CalculateSteeringForce(FVector TargetLocation);

    // 设置路径点
    UFUNCTION(BlueprintCallable, Category = "Steering")
        void SetPathPoints(const TArray<FVector>& NewPath) { PathPoints = NewPath; CurrentWaypointIndex = 0; }

    // 根据目标位置计算NavMesh路径点
    UFUNCTION(BlueprintCallable, Category = "Steering")
        bool CalculatePathToLocation(const FVector& TargetLocation);

    // 获取拥有者的Actor
    AActor* GetOwnerActor() const { return GetOwner(); }

private:
    // 核心行为函数
    FVector Seek(const FVector& Target);
    FVector AvoidObstacles();
    FVector CalculateAvoidanceForce(const FVector& ActorLocation, const FRotator& ActorRotation, const TArray<FVector>& ObstaclePositions);
    FVector FollowPath();
    FVector Separation();
	
};
