// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SV_UpdateDistanceToEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API USV_UpdateDistanceToEnemy : public UBTService
{
	GENERATED_BODY()

protected : 
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetEnemyKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector DistanceToTargetKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector WeaponShortRangeKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector WeaponMediumRangeKey;
};
