// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AIs/Decorators/DC_IsFacingTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"


bool UDC_IsFacingTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (AAIController * AIController = Cast<AAIController>(OwnerComp.GetOwner()))
	{
		APawn* ControlledPawn = AIController->GetPawn();
		if ((ControlledPawn != nullptr) && 
			(BlackboardComponent != nullptr))
		{
			AActor* TargetEnemy = (AActor*)BlackboardComponent->GetValueAsObject(TargetEnemyKey.SelectedKeyName);
			if (TargetEnemy != nullptr)
			{
				FVector Location = ControlledPawn->GetActorLocation();
				FVector ZAdjustedTargetLocation = TargetEnemy->GetActorLocation();
				ZAdjustedTargetLocation.Z = Location.Z;

				FRotator ToTargetRotation = UKismetMathLibrary::FindLookAtRotation(Location, ZAdjustedTargetLocation);
				FRotator ForwardRotation = UKismetMathLibrary::MakeRotFromX(ControlledPawn->GetActorForwardVector());

				return UKismetMathLibrary::EqualEqual_RotatorRotator(ToTargetRotation, ForwardRotation, TestSimilarity);
			}
		}
	}
	return false;
}

