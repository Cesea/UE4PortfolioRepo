// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/NPCs/NPCBase.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "Public/Actors/NPCs/NPCControllerBase.h"

#include "Public/Actors/Players/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Public/Components/PatrolComponent.h"

#include "Public/Components/InputBufferComponent.h"
#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/InventoryComponent.h"	
#include "Public/Components/AIRotationControlComponent.h"	
#include "Public/Components/ConversationComponent.h"
#include "Public/Components/StateMachineComponent.h"
#include "Public/Components/StatComponent.h"
#include "Public/Components/CustomizingComponent.h"

#include "Public/ASGameInstance.h"
#include "Public/Utils/ASSaveGame.h"

#define LOCTEXT_NAMESPACE "NPCBase"

ANPCBase::ANPCBase()
{
	ConversationComponent = CreateDefaultSubobject<UConversationComponent>("ConversationComponent");
	CustomizingComponent = CreateDefaultSubobject<UCustomizingComponent>("CustomizingComponent");

	InteractionSphere->SetCollisionProfileName("Interaction");

	bIsHostile = false;
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	//Check Save, if already dead then Destroy
	UASGameInstance* GameInstance = (UASGameInstance*)UGameplayStatics::GetGameInstance((UObject* )GetWorld());
	if (UASSaveGame * GameStatus = GameInstance->GetCurrentGameStatus())
	{
		const FCharacterSaveData* SaveData = GameStatus->NPCsSaveData.Find(EnemyData->Id);
		if (GameStatus->NPCsSaveData.Contains(EnemyData->Id))
		{
			if (SaveData->bIsDead)
			{
				EquipmentComponent->DestroyAllEquipments();
				Destroy();
			}
			else
			{
				LoadNPC();
			}
		}
		else
		{
			SaveNPC(GameStatus);
		}
	}
}


void ANPCBase::Interact_Implementation(AActor* Actor)
{
	//살아있을때
	if (HealthStatComponent->GetCurrentValue() == 0.0f)
	{
			if (AMyPlayerController * PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{
				PlayerController->ToggleShops(this, false);
			}
	}
	else
	{
		UBlackboardComponent* Blackboard = ControllerRef->GetBlackboardComponent();
		switch ((EAIState)Blackboard->GetValueAsEnum("AIState"))
		{
		case EAIState::EAS_Idling:
		case EAIState::EAS_Patrolling:
		{
			ConversationComponent->StartConversation();
		}break;
		}
	}
}

void ANPCBase::ShowFeedbackWidget_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("NPC Show Feedback"));
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (AMyPlayerController * MyPC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("Message"), MessageToShow);
			Arguments.Add(TEXT("Name"), EnemyData->Name);
			MyPC->ShowInteractionWidget(FText::Format(LOCTEXT("FeedbackText", "{Message} {Name}"), Arguments));
		}
	}
}

void ANPCBase::HideFeedbackWidget_Implementation(AActor* Actor)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (AMyPlayerController * MyPC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			MyPC->HideInteractionWidget();
		}
	}
}


void ANPCBase::OnBeingHitted_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage)
{
	Super::OnBeingHitted_Implementation(HitResult, Attacker, Damage);
	bIsHostile = true;
}

void ANPCBase::OnAIStateEnter_Implementation(EAIState NewState)
{
	switch (NewState)
	{
	case EAIState::EAS_Idling:
	case EAIState::EAS_Patrolling:
	{
		InteractionSphere->SetCollisionProfileName("Interaction");
	}break;
	case EAIState::EAS_Dead :
	{
	}break;
	case EAIState::EAS_Chasing:
	case EAIState::EAS_Combating:
	case EAIState::EAS_Stuning:
	{
		InteractionSphere->SetCollisionProfileName("NoCollision");
	} break;
	case EAIState::EAS_Searcging:
		break;
	case EAIState::EAS_Fleeing:
		break;
	case EAIState::EAS_Defensing:
		break;
	default:
		break;
	}
}

void ANPCBase::OnAIStateExit_Implementation(EAIState ExitState)
{
	switch (ExitState)
	{
	case EAIState::EAS_Idling:
	case EAIState::EAS_Patrolling:
	{
	} break;
	case EAIState::EAS_Chasing:
		break;
	case EAIState::EAS_Combating:
		break;
	case EAIState::EAS_Searcging:
		break;
	case EAIState::EAS_Fleeing:
		break;
	case EAIState::EAS_Stuning:
		break;
	case EAIState::EAS_Defensing:
		break;
	default:
		break;
	}
}

void ANPCBase::OnDead_Implementation()
{
	Super::OnDead_Implementation();
	UASGameInstance* GameInstance = (UASGameInstance*)UGameplayStatics::GetGameInstance((UObject* )GetWorld());
	if (UASSaveGame* GameStatus = GameInstance->GetCurrentGameStatus())
	{
		if (GameStatus->NPCsSaveData.Contains(EnemyData->Id))
		{
			GameStatus->NPCsSaveData[EnemyData->Id].bIsDead = true;
		}
	}
}

void ANPCBase::SaveNPC(UASSaveGame* SaveGame)
{
	if (SaveGame != nullptr)
	{
		FCharacterSaveData& SaveData = SaveGame->NPCsSaveData.FindOrAdd(EnemyData->Id);
		if (!SaveData.bIsDead)
		{
			SaveData.Transform = GetActorTransform();

			SaveData.InventorySave.Gold = InventoryComponent->GetGold();
			SaveData.InventorySave.InventoryItems = InventoryComponent->GetInventories();

			SaveData.StatSave.CurrentHealth = HealthStatComponent->GetCurrentValue();
			SaveData.StatSave.MaxHealth = HealthStatComponent->GetMaxValue();
			SaveData.StatSave.CurrentStamina = StaminaStatComponent->GetCurrentValue();
			SaveData.StatSave.MaxStamina = StaminaStatComponent->GetMaxValue();
		}
	}

}

void ANPCBase::LoadNPC()
{
	UASGameInstance* GameInstance = (UASGameInstance*)UGameplayStatics::GetGameInstance((UObject* )GetWorld());
	if (UGameplayStatics::DoesSaveGameExist(GameInstance->GetCurrentSlotName(), 0))
	{
		UASSaveGame* LoadedGame = (UASSaveGame*)UGameplayStatics::LoadGameFromSlot(GameInstance->GetCurrentSlotName(), 0);
		if (LoadedGame != nullptr)
		{
			const FCharacterSaveData* SaveData = LoadedGame->NPCsSaveData.Find(EnemyData->Id);

			SetActorTransform(SaveData->Transform);

			HealthStatComponent->InitializeValues(SaveData->StatSave.CurrentHealth, SaveData->StatSave.MaxHealth);
			StaminaStatComponent->InitializeValues(SaveData->StatSave.CurrentStamina, SaveData->StatSave.MaxStamina);

			InventoryComponent->SetInventory(SaveData->InventorySave.InventoryItems);
			InventoryComponent->SetGold(SaveData->InventorySave.Gold);
		}
	}
}

