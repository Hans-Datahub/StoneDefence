// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BTService/SeparationDetection.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Core/RuleOfAIController.h"
#include "Character/SteeringBehavior/SteeringBehaviorComponent.h"


void USeparationDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent();
    if (!MyBlackBoard) return;

    ARuleOfCharacter* Owner = Cast<ARuleOfCharacter>(MyBlackBoard->GetValueAsObject(Blackboard_SelfActor.SelectedKeyName));
    if (!Owner) return;

    USteeringBehaviorComponent* SteeringComponent = Cast<USteeringBehaviorComponent>(
        Owner->GetComponentByClass(USteeringBehaviorComponent::StaticClass()));

    FVector OwnerLocation = Owner->GetActorLocation();

    // 检测是否有单位过于靠近
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(Owner);

    TArray<AActor*> NearbyUnits;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        OwnerLocation,
        SteeringComponent->IdleSeparationTriggerDistance,
        ObjectTypes,
        SteeringComponent->SeparationActorClass ? SteeringComponent->SeparationActorClass : ARuleOfCharacter::StaticClass(),
        IgnoredActors,
        NearbyUnits
    );

    if (NearbyUnits.Num() > 0)
        MyBlackBoard->SetValueAsBool(Blackboard_NeedToAvoid.SelectedKeyName, true);
    else
        MyBlackBoard->SetValueAsBool(Blackboard_NeedToAvoid.SelectedKeyName, false);
}
