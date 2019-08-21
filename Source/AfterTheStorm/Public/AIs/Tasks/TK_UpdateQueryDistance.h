// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TK_UpdateQueryDistance.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UTK_UpdateQueryDistance : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector DistanceMinKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector DistanceMaxKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector WeaponShortRangeKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector WeaponMediumRangeKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector CurrentDistanceStateKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector PursuingDistanceStateKey;
};
