// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/NPCs/EnemyBase.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "TimerManager.h"

#include "Public/Actors/NPCs/EnemyControllerBase.h"
#include "Public/Actors/EquipmentBase.h" 

#include "Public/Actors/Players/PlayerCharacter.h"
#include "Public/Actors/Players/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Public/Components/PatrolComponent.h"

#include "Public/Components/InputBufferComponent.h"
#include "Public/Components/PersonalityComponent.h"
#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/InventoryComponent.h"	
#include "Public/Components/AIRotationControlComponent.h"	
#include "Public/Components/ConversationComponent.h"
#include "Public/Components/StateMachineComponent.h"
#include "Public/Components/StatComponent.h"
#include "Public/Components/ExpComponent.h"
#include "Public/Components/MontageManagerComponent.h"

#include "Public/ASGameInstance.h"

#include "Public/AIs/Tasks/TK_UpdateAIState.h"


#define LOCTEXT_NAMESPACE "EnemyBase"

AEnemyBase::AEnemyBase()
{
	PatrolComponent = CreateDefaultSubobject<UPatrolComponent>("PatrolComponent");

	AIRotationControlComponent = CreateDefaultSubobject<UAIRotationControlComponent>("AIRotationControlComponent");

	PersonalityComponent = CreateDefaultSubobject<UPersonalityComponent>("PersonalityComponent");

	TargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("TargetWidget");
	TargetWidgetComponent->SetupAttachment(RootComponent);

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidgetComponent->SetupAttachment(RootComponent);

	bIsTargeted = false;
	bIsHostile = true;

	MessageToShow = FText::FromString("Press E to take items from");

	InteractionSphere->SetCollisionProfileName("NoCollision");
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = Cast<AEnemyControllerBase>(GetController());
	check(ControllerRef != nullptr);

	if (UASGameInstance * Instance = Cast<UASGameInstance>(GetGameInstance()))
	{
		EnemyData = Instance->GetEnemyDB()->FindRow<FEnemyData>(GetID(), "");
		check(EnemyData != nullptr);
	}

	ControllerRef->UpdateSightConfig(EnemyData->VisionAngleInDegrees, EnemyData->SightRadius, EnemyData->LoseSightRadius);

	HealthStatComponent->InitializeValues(EnemyData->Health, EnemyData->Health);
	StaminaStatComponent->InitializeValues(EnemyData->Stamina, EnemyData->Stamina);

	InputBufferComponent->OnConsumedEvent.AddUObject(this, &AEnemyBase::OnInputBufferConsumed);
	InputBufferComponent->OnCloseEvent.AddUObject(this, &AEnemyBase::OnInputBufferClose);

	StateMachineComponent->SetState(EState::ES_Stand);

	InventoryComponent->EquipEquipments();

	HealthWidgetComponent->SetVisibility(false);
	TargetWidgetComponent->SetVisibility(false);

}

void AEnemyBase::OnEquipmentEquipped_Implementation(EEquipmentType EquipmentType, AEquipmentBase* Equipment)
{
	Super::OnEquipmentEquipped_Implementation(EquipmentType, Equipment);
}

void AEnemyBase::OnEquipmentUnequipped_Implementation(EEquipmentType EquipmentType, AEquipmentBase* Equipment)
{
	Super::OnEquipmentUnequipped_Implementation(EquipmentType, Equipment);
}

//적들은 오직 죽었을때만 루트 인터렉션을 할 수 있다
void AEnemyBase::Interact_Implementation(AActor* Actor)
{
	switch (StateMachineComponent->GetState())
	{
		case EState::ES_Dead: 
		{
			if (AMyPlayerController * PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{
				PlayerController->ToggleShops(this, false);
			}
		}break;
	}
}

void AEnemyBase::ShowFeedbackWidget_Implementation(AActor* Actor)
{
	if (APlayerCharacter * PlayerCharacter = Cast<APlayerCharacter>(Actor))
	{
		if (AMyPlayerController * MyPC = Cast<AMyPlayerController>(PlayerCharacter->GetController()))
		{
			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("Message"), MessageToShow);
			Arguments.Add(TEXT("Name"), EnemyData->Name);
			MyPC->ShowInteractionWidget(FText::Format(LOCTEXT("FeedbackText", "{Message} {Name}"), Arguments));
		}
	}
}

void AEnemyBase::HideFeedbackWidget_Implementation(AActor* Actor)
{
	if (APlayerCharacter * PlayerCharacter = Cast<APlayerCharacter>(Actor))
	{
		if (AMyPlayerController * MyPC = Cast<AMyPlayerController>(PlayerCharacter->GetController()))
		{
			MyPC->HideInteractionWidget();
		}
	}
}

void AEnemyBase::OnTargeted_Implementation(AActor* LookingActor)
{
	bIsTargeted = true;
	TargetWidgetComponent->SetVisibility(true, true);
	HealthWidgetComponent->SetVisibility(true, true);

	if (HideInfoWidgetsTimerHandle.IsValid())
	{
		HideInfoWidgetsTimerHandle.Invalidate();
	}
}

void AEnemyBase::OnReleasedTarget_Implementation(AActor* LookingActor)
{
	bIsTargeted = false;
	TargetWidgetComponent->SetVisibility(false, false);
	HealthWidgetComponent->SetVisibility(false, false);
}

void AEnemyBase::Stun_Implementation(EMovementDirection Direction, bool bIsHeavy)
{
	Super::Stun_Implementation(Direction, bIsHeavy);

	if (StateMachineComponent->GetState() == EState::ES_Stun)
	{
		UBlackboardComponent* BlackboardComponent = ControllerRef->GetBlackboardComponent();
		if (BlackboardComponent != nullptr)
		{
			OnAIStateExit((EAIState)BlackboardComponent->GetValueAsEnum("AIState"));
			BlackboardComponent->SetValueAsEnum("AIState", (uint8)EAIState::EAS_Stuning);
			OnAIStateEnter(EAIState::EAS_Stuning);
		}
	}
}

void AEnemyBase::OnBeingHitted_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage)
{
	Super::OnBeingHitted_Implementation(HitResult, Attacker, Damage);

	UBlackboardComponent* BlackboardComponent = ControllerRef->GetBlackboardComponent();
	if (BlackboardComponent != nullptr)
	{
		ControllerRef->SetEnemy(Attacker);
		BlackboardComponent->SetValueAsObject("TargetEnemy", Attacker);
	}

	bIsHostile = true;

}

void AEnemyBase::OnDead_Implementation()
{
	Super::OnDead_Implementation();

	ITargetable::Execute_OnReleasedTarget(this, nullptr);

	InteractionSphere->SetCollisionProfileName("Interaction");
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	ControllerRef->GetBrainComponent()->StopLogic("");

	DetachFromControllerPendingDestroy();

	GetMesh()->PlayAnimation(MontageManagerComponent->GetDeathAnimation(), false);

	MessageToShow = FText::FromString("Press E to take items from");

	if (ACharacter * PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UExpComponent* ExpComponent = PlayerCharacter->FindComponentByClass<UExpComponent>();
		ExpComponent->AddExp(EnemyData->ExpAmount);
	}
}

void AEnemyBase::ToggleWeapon_Implementation()
{
	Super::ToggleWeapon_Implementation();
	if (StanceState == EStanceState::ESS_Armed)
	{
		UBlackboardComponent* Blackboard = ControllerRef->GetBlackboardComponent();
		AWeaponBase* Weapon = Cast<AWeaponBase>(EquipmentComponent->FindWeaponActor());
		if (Blackboard != nullptr && Weapon != nullptr)
		{
			Blackboard->SetValueAsFloat("WeaponShortRange", Weapon->GetShortRange());
			Blackboard->SetValueAsFloat("WeaponMediumRange", Weapon->GetMediumRange());
		}
	}
}


void AEnemyBase::OnAIStateEnter_Implementation(EAIState NewState)
{
	switch (NewState)
	{
	case EAIState::EAS_Dead :
	{
	}break;
	}
}


void AEnemyBase::OnAIStateExit_Implementation(EAIState ExitState)
{
}

void AEnemyBase::OnHealthStatChanged_Implementation(float Current, float Max)
{
	if (!bIsTargeted)
	{
		HealthWidgetComponent->SetVisibility(true);
		if (HideInfoWidgetsTimerHandle.IsValid())
		{
			HideInfoWidgetsTimerHandle.Invalidate();
		}
		GetWorldTimerManager().SetTimer(HideInfoWidgetsTimerHandle, this, &AEnemyBase::HideHealthWidgets, 5.0f, false);
	}
}

void AEnemyBase::OnHealthStatStateChanged_Implementation(EStatState NewState, float Percent)
{
	UBlackboardComponent* BlackboardComponent = ControllerRef->GetBlackboardComponent();
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsEnum("HealthState", (uint8)NewState);
	}
}

void AEnemyBase::OnStaminaStatChanged_Implementation(float Current, float Max)
{
}

void AEnemyBase::OnStaminaStatStateChanged_Implementation(EStatState NewState, float Percent)
{
	UBlackboardComponent* BlackboardComponent = ControllerRef->GetBlackboardComponent();
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsEnum("StaminaState", (uint8)NewState);
	}
}

void AEnemyBase::HideHealthWidgets()
{
	HealthWidgetComponent->SetVisibility(false);
}


FVector AEnemyBase::GetEvadeDirection()
{
	UBlackboardComponent* BlackboardComponent = ControllerRef->GetBlackboardComponent();
	if (BlackboardComponent != nullptr)
	{
		FVector TargetLocation = BlackboardComponent->GetValueAsVector("TargetLocation");
		FVector Diff = (TargetLocation - GetActorLocation());
		return Diff.GetSafeNormal();
	}
	return FVector::ForwardVector;
}
