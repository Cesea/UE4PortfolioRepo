// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AIs/Decorators/DC_Random.h"


bool UDC_Random::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const 
{
	return ((float)FMath::Rand() / (float)RAND_MAX) < AcceptPercent;
}

