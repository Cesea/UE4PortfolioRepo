// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "DC_IsFacingTarget.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UDC_IsFacingTarget : public UBTDecorator
{
	GENERATED_BODY()
	
protected :
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetEnemyKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TestSimilarity;
};
