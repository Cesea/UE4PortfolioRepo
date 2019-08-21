// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/ConversationComponent.h"

#include "GameFramework/Actor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

// Sets default values for this component's properties
UConversationComponent::UConversationComponent()
{
}


// Called when the game starts
void UConversationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UConversationComponent::StartConversation()
{
	if (APawn * Owner = (APawn*)GetOwner())
	{
		if (AAIController * AIController = Cast<AAIController>(Owner->GetController()))
		{
			UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
			if (BlackboardComponent != nullptr)
			{
				UBehaviorTreeComponent* BehaviorTreeComp = (UBehaviorTreeComponent*)AIController->GetComponentByClass(UBehaviorTreeComponent::StaticClass());
				PrevTree = BehaviorTreeComp->GetRootTree();
				PrevBlackboard = BlackboardComponent->GetBlackboardAsset();
			}

			AIController->UseBlackboard(ConversationBlackboard, BlackboardComponent);
			AIController->RunBehaviorTree(ConversationTree);
		}
	}
}

void UConversationComponent::FinishConversation()
{
	if (APawn * Owner = (APawn*)GetOwner())
	{
		if (AAIController * AIController = Cast<AAIController>(Owner->GetController()))
		{
			AIController->GetBrainComponent()->StopLogic("");

			UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
			AIController->UseBlackboard(PrevBlackboard, BlackboardComponent);
			AIController->RunBehaviorTree(PrevTree);
		}
	}
}

