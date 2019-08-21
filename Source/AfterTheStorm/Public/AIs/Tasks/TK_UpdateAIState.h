// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"

#include "Public/Enums/CharacterEnums.h"

#include "TK_UpdateAIState.generated.h"
/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UTK_UpdateAIState : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector AIStateKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIState NewAIState;
};
