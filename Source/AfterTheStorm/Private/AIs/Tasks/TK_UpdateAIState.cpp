// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AIs/Tasks/TK_UpdateAIState.h"

#include "BehaviorTree/BlackboardComponent.h"


#include "Public/Actors/NPCs/EnemyBase.h"
#include "Public/Actors/NPCs/EnemyControllerBase.h"

EBTNodeResult::Type UTK_UpdateAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyControllerBase * EnemyController = Cast<AEnemyControllerBase>(OwnerComp.GetOwner()))
	{
		if (AEnemyBase * EnemyBase = Cast<AEnemyBase>(EnemyController->GetPawn()))
		{
			EnemyBase->OnAIStateExit((EAIState)OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AIStateKey.SelectedKeyName));

			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIStateKey.SelectedKeyName, (uint8)NewAIState);
			EnemyBase->OnAIStateEnter(NewAIState);
		}
	}
	return EBTNodeResult::Type::Succeeded;
}

