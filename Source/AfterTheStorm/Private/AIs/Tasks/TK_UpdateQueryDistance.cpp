// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AIs/Tasks/TK_UpdateQueryDistance.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Public/Enums/CharacterEnums.h"


EBTNodeResult::Type UTK_UpdateQueryDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard != nullptr)
	{
		float WeaponShortRange = Blackboard->GetValueAsFloat(WeaponShortRangeKey.SelectedKeyName);
		float WeaponMediumRange =Blackboard->GetValueAsFloat(WeaponMediumRangeKey.SelectedKeyName);
		float OverRange = 900.0f;

		switch ((EDistanceToTarget)Blackboard->GetValueAsEnum(PursuingDistanceStateKey.SelectedKeyName))
		{
		case EDistanceToTarget::EDT_Short: 
		{
			Blackboard->SetValueAsFloat(DistanceMinKey.SelectedKeyName, 0.0f);
			Blackboard->SetValueAsFloat(DistanceMaxKey.SelectedKeyName, WeaponShortRange);

		}break;
		case EDistanceToTarget::EDT_Medium: 
		{
			Blackboard->SetValueAsFloat(DistanceMinKey.SelectedKeyName, WeaponShortRange);
			Blackboard->SetValueAsFloat(DistanceMaxKey.SelectedKeyName, WeaponMediumRange);
		}break;
		case EDistanceToTarget::EDT_Far: 
		{
			Blackboard->SetValueAsFloat(DistanceMinKey.SelectedKeyName, WeaponMediumRange);
			Blackboard->SetValueAsFloat(DistanceMaxKey.SelectedKeyName, OverRange);
		}break;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
