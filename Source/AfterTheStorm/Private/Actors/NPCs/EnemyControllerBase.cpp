// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/NPCs/EnemyControllerBase.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "Public/CharacterBase.h"

#include "Public/Components/TargetingComponent.h"
#include "Public/Components/PersonalityComponent.h"
#include "Public/Interfaces/Attackable.h"

AEnemyControllerBase::AEnemyControllerBase()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

void AEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();
	SetGenericTeamId(FGenericTeamId(FactionID));
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyControllerBase::OnTargetPerceptionUpdated);
}


void AEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UPersonalityComponent* Personality = InPawn->FindComponentByClass<UPersonalityComponent>();
	if (Personality != nullptr)
	{
		RunBehaviorTree(Personality->BehaviorTreeToRun);
	}
}

ETeamAttitude::Type AEnemyControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn * OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID == 200)
			{
				return ETeamAttitude::Neutral;
			}
			else if (OtherTeamID == GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}
			else 
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

void AEnemyControllerBase::SetEnemy(AActor* NewEnemy)
{
	if (TargetEnemy != NewEnemy)
	{
		TargetEnemy = NewEnemy;
	}
}

void AEnemyControllerBase::UpdateSightConfig(float VisionAngleDegrees, float Radius, float LoseRadius)
{
	FAISenseID SightSenseID = UAISense::GetSenseID<UAISense_Sight>();
	UAISenseConfig_Sight* SightConfig =  (UAISenseConfig_Sight*)AIPerceptionComponent->GetSenseConfig(SightSenseID);
	if (SightConfig != nullptr)
	{
		SightConfig->PeripheralVisionAngleDegrees = VisionAngleDegrees;
		SightConfig->SightRadius = Radius;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = Radius;
		SightConfig->LoseSightRadius = LoseRadius;
	}
}

void AEnemyControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Blackboard != nullptr && 
		BrainComponent->IsRunning())
	{
		if (Stimulus.Tag == "Noise")
		{
			bool IsOtherTeam = IAttackable::Execute_IsOtherTeam(Actor, IAttackable::Execute_GetTeamId(GetPawn()));
			bool IsHostile = IAttackable::Execute_IsHostile(GetPawn());
			if (IsHostile && IsOtherTeam)
			{
				SetEnemy(Actor);
			}
			Blackboard->SetValueAsObject("TargetEnemy", TargetEnemy);
		}
		else
		{
			if (ACharacterBase * CharacterBase = Cast<ACharacterBase>(Actor))
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					bool IsOtherTeam = IAttackable::Execute_IsOtherTeam(CharacterBase, IAttackable::Execute_GetTeamId(GetPawn()));
					bool IsHostile = IAttackable::Execute_IsHostile(GetPawn());
					if (IsHostile && IsOtherTeam)
					{
						SetEnemy(Actor);
					}
				}
				else
				{
					APawn* ControlledPawn = GetPawn();
					if (TargetEnemy != nullptr && ControlledPawn != nullptr)
					{
						float DistanceToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), TargetEnemy->GetActorLocation());
						if (DistanceToTarget > Blackboard->GetValueAsFloat("WeaponMediumRange"))
						{
							Blackboard->SetValueAsVector("TargetLastLocation", TargetEnemy->GetActorLocation());
							SetEnemy(nullptr);
						}
					}
				}
				Blackboard->SetValueAsObject("TargetEnemy", TargetEnemy);
			}
		}
	}

}
