// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AIs/Tasks/TK_FindNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Public/Components/PatrolComponent.h"
#include "AIController.h"


EBTNodeResult::Type UTK_FindNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController * AIController = Cast<AAIController>(OwnerComp.GetOwner()))
	{
		APawn* ControlledPawn = AIController->GetPawn();
		if (ControlledPawn != nullptr)
		{
			UPatrolComponent* PatrolComponent = ControlledPawn->FindComponentByClass<UPatrolComponent>();
			if (PatrolComponent != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, PatrolComponent->ToNextPatrolPoint());
				return EBTNodeResult::Type::Succeeded;
			}
		}
	}

	return EBTNodeResult::Type::Failed;
}

void UTK_FindNextPatrolPoint::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

//void UTK_FindNextPatrolPoint::OnGameplayTaskDeactivated(UGameplayTask& Task)
//{
//	Super::OnGameplayTaskDeactivated(Task);
//}
