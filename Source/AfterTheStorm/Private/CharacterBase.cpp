// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Public/Components/InputBufferComponent.h"
#include "Public/Components/StateMachineComponent.h"
#include "Public/Components/InteractionComponent.h"
#include "Public/Components/MovementControlComponent.h"
#include "Public/Components/StatComponent.h"
#include "Public/Components/InventoryComponent.h"
#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/MontageManagerComponent.h"

#include "Public/Actors/EquipmentBase.h"

#include "Public/Combat/AttackMove.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set orient to movement true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetCollisionProfileName("Interaction");

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnInteractionSphereBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ACharacterBase::OnInteractionSphereEndOverlap);

	//Setup Interaction Component
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");

	//Setup InputBuffer Component
	InputBufferComponent = CreateDefaultSubobject<UInputBufferComponent>("InputBufferComponent");

	//Setup StateMachine Component
	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>("StateMachineComponent");

	//Setup MovementControl Component 
	MovementControlComponent = CreateDefaultSubobject<UMovementControlComponent>("MovementControlComponent");

	//Setup HP StatComponent
	HealthStatComponent = CreateDefaultSubobject<UStatComponent>("HealthComponent");
	HealthStatComponent->ComponentTags.Add(FName("Health"));
	//Setup Stamina StatComponent
	StaminaStatComponent = CreateDefaultSubobject<UStatComponent>("StaminaComponent");
	StaminaStatComponent->ComponentTags.Add(FName("Stamina"));
	StaminaStatComponent->SetRegenDelay(0.8f);
	StaminaStatComponent->SetRegenPerSeconds(35.0f);

	//Setup InventoryComponent
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	//Setup InventoryComponent
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");

	//Setup MontageManager Component
	MontageManagerComponent = CreateDefaultSubobject<UMontageManagerComponent>("MontageManager");

	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;

	bIsVulnerable = true;
	bIsDefensing = false;
	bIsHostile = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HealthStatComponent->OnStatChanged.AddDynamic(this, &ACharacterBase::OnHealthStatChanged);
	HealthStatComponent->OnStatStateChanged.AddDynamic(this, &ACharacterBase::OnHealthStatStateChanged);
	HealthStatComponent->OnStatZeroEvent.AddDynamic(this, &ACharacterBase::OnDead);
	StaminaStatComponent->OnStatChanged.AddDynamic(this, &ACharacterBase::OnStaminaStatChanged);
	StaminaStatComponent->OnStatStateChanged.AddDynamic(this, &ACharacterBase::OnStaminaStatStateChanged);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StateMachineComponent->GetState() == EState::ES_Jump && !GetCharacterMovement()->IsFalling())
	{
		StateMachineComponent->SetState(EState::ES_Stand);
	}
}

void ACharacterBase::HandleHit_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage)
{
	if (CanBeHitted(Attacker))
	{
		if(bIsDefensing)
		{
			OnDefenseSuccess(Attacker);
		}
		else
		{
			OnBeingHitted(HitResult, Attacker, Damage);
		}
	}
}

void ACharacterBase::Impact_Implementation()
{
}

bool ACharacterBase::IsAlive_Implementation()
{
	return StateMachineComponent->GetState() != EState::ES_Dead;
}

void ACharacterBase::OnTargeted_Implementation(AActor* LookingActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Base OnTargeted"));
}

void ACharacterBase::OnReleasedTarget_Implementation(AActor* LookingActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Base OnLooseTarget"));
}

void ACharacterBase::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta)
{
	UE_LOG(LogTemp, Warning, TEXT("I'm Walking Off Ledge"))
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->StopAllMontages(0.15f);
	}
	if (StateMachineComponent != nullptr)
	{
		StateMachineComponent->SetState(EState::ES_Jump);
	}
}

void ACharacterBase::MainAttack_Implementation(EInputBufferKey InputKey)
{
	if (CanAttack())
	{
		AWeaponBase* Weapon = EquipmentComponent->FindWeaponActor();
		if (Weapon != nullptr)
		{
			//If Attack Success
			UAttackMove* AttackMove = Weapon->Attack(InputKey);
			if (AttackMove != nullptr && AttackMove->AttackType != EAttackType::EAT_Invalid)
			{
				if (StaminaStatComponent->GetCurrentValue() > 0)
				{
					StaminaStatComponent->SubStat(AttackMove->StaminaCost);

					UAnimMontage* MontageToPlay = MontageManagerComponent->GetAttackMontage(Weapon->GetWeaponType(), AttackMove->AttackType);
					UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
					if (AnimInstance != nullptr && 
						MontageToPlay != nullptr)
					{
						AnimInstance->Montage_Play(MontageToPlay);

						AttackType = (EAttackType)InputKey;
						StateMachineComponent->SetState(EState::ES_Attack);
					}
				}
			}
		}
	}
}

void ACharacterBase::Defense_Implementation()
{
	if (CanDefense())
	{
		AWeaponBase* EquippedWeapon = EquipmentComponent->FindWeaponActor();
		UAnimMontage* DefenseMontage = MontageManagerComponent->GetDefenseMontage(EquippedWeapon->GetWeaponType());
		if (EquippedWeapon != nullptr && DefenseMontage != nullptr)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(DefenseMontage);
			StateMachineComponent->SetState(EState::ES_Attack);

			StaminaStatComponent->SubStat(10);
		}
	}
}

void ACharacterBase::Stun_Implementation(EMovementDirection Direction, bool bIsHeavy)
{
	if (CanBeStuned())
	{
		InputBufferComponent->bIsOpen = true;
		InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_Invalid);
		StateMachineComponent->SetState(EState::ES_Stun);

		UAnimMontage* StunMontage = bIsHeavy ? 
			MontageManagerComponent->GetHeavyStunMontage(Direction, StanceState) : 
			MontageManagerComponent->GetLightStunMontage();
		if (StunMontage != nullptr)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(StunMontage);
		}

		if (bIsWeaponToggling)
		{
			AWeaponBase* Weapon = EquipmentComponent->FindWeaponActor();
			if (Weapon != nullptr)
			{
				if (StanceState == EStanceState::ESS_Armed)
				{
					Weapon->PullOut();
				}
				else if (StanceState == EStanceState::ESS_Unarmed)
				{
					Weapon->PutIn();
				}
			}
		}
	}
}

void ACharacterBase::Evade_Implementation(FVector EvadeDirection)
{
	if (CanEvade())
	{
		StateMachineComponent->SetState(EState::ES_Evade);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		AddMovementInput(EvadeDirection);

		GetMesh()->GetAnimInstance()->Montage_Play(MontageManagerComponent->GetEvadeMontage());


		StaminaStatComponent->SubStat(10);
	}
}

void ACharacterBase::DoJump_Implementation()
{
	if (CanDoJump())
	{
		Jump();
		StateMachineComponent->SetState(EState::ES_Jump);
	}
}

void ACharacterBase::ToggleWeapon_Implementation()
{
	if (CanToggleWeapon())
	{
		bIsWeaponToggling = true;
		if (GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.2f)
		{
			bWasMovingInToggle = true;
			IgnoreRootmotion();
		}
		else
		{
			bWasMovingInToggle = false;
		}

		if (AWeaponBase * Weapon = EquipmentComponent->FindWeaponActor())
		{
			if (StanceState == EStanceState::ESS_Unarmed)
			{
				StanceState = EStanceState::ESS_Armed;
			}
			else if (StanceState == EStanceState::ESS_Armed)
			{
				StanceState = EStanceState::ESS_Unarmed;
			}
			UAnimMontage* Montage = MontageManagerComponent->GetEquipHolsterMontage(Weapon->GetWeaponType(), StanceState);
			float MontageLength = GetMesh()->GetAnimInstance()->Montage_Play(Montage);
			GetWorldTimerManager().SetTimer(EnableRootmotionTimerHandle, this, &ACharacterBase::ResetRootmotion, MontageLength, false);
		}
		StateMachineComponent->SetState(EState::ES_Stand);
	}
}

void ACharacterBase::InteractWith_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Interact"));
	if (InteractionComponent != nullptr)
	{
		InteractionComponent->Interact();

	}
}

void ACharacterBase::OnDead_Implementation()
{
	StateMachineComponent->SetState(EState::ES_Dead);
	GetCharacterMovement()->DisableMovement();
}

void ACharacterBase::StartSprint()
{
	if (CanSprint())
	{
		if (StanceState == EStanceState::ESS_Armed)
		{
			MovementControlComponent->SetMovementState(EMovementState::EMS_Run);
		}
		else if(StanceState == EStanceState::ESS_Unarmed)
		{
			MovementControlComponent->SetMovementState(EMovementState::EMS_Sprint);
		}
	}
}
void ACharacterBase::StopSprint()
{
	if (StanceState == EStanceState::ESS_Unarmed)
	{
		MovementControlComponent->SetMovementState(EMovementState::EMS_Run);
	}
}


void ACharacterBase::ToggleWalkRun()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleWalkRun"));
	EMovementState CurrentMovementState = MovementControlComponent->GetMovementState();
	if (CurrentMovementState == EMovementState::EMS_Walk)
	{
		MovementControlComponent->SetMovementState(EMovementState::EMS_Run);
	}
	else if (CurrentMovementState == EMovementState::EMS_Run)
	{
		MovementControlComponent->SetMovementState(EMovementState::EMS_Walk);
	}
}

bool ACharacterBase::CanAttack()
{
	if (StateMachineComponent != nullptr)
	{
		return (StateMachineComponent->GetState() == EState::ES_Stand) && 
			(StanceState != EStanceState::ESS_Unarmed) &&
			(AttackType == EAttackType::EAT_Invalid);
	}
	return false;
}


bool ACharacterBase::CanDoJump()
{
	if (StateMachineComponent != nullptr)
	{
		return StateMachineComponent->GetState() == EState::ES_Stand;
	}
	return false;
}

bool ACharacterBase::CanBeHitted(AActor* Attacker)
{
	IAttackable* Attackable = Cast<IAttackable>(Attacker);

	bool bIsOtherTeam = false;
	if (Attackable != nullptr)
	{
		bIsOtherTeam = IAttackable::Execute_IsOtherTeam(Attacker, IAttackable::Execute_GetTeamId(this));
	}
	return (HealthStatComponent->GetCurrentValue() > 0.0f) && (bIsVulnerable) && bIsOtherTeam;
}

bool ACharacterBase::CanBeStuned()
{
	return 	(StateMachineComponent->GetState() != EState::ES_Jump) && StateMachineComponent->GetState() != EState::ES_Stun;
}

bool ACharacterBase::CanEvade()
{

	return (StateMachineComponent->GetState() == EState::ES_Stand) && (StaminaStatComponent->GetCurrentValue() > 0.0f);
}

bool ACharacterBase::CanDefense()
{
	return (StateMachineComponent->GetState() == EState::ES_Stand) && (StaminaStatComponent->GetCurrentValue() > 0.0f);
}

bool ACharacterBase::CanToggleWeapon()
{
	if (!bIsWeaponToggling)
	{
		if (MovementControlComponent->GetMovementState() == EMovementState::EMS_Sprint)
		{
			MovementControlComponent->SetMovementState(EMovementState::EMS_Run);
			return true;
		}
		else
		{
			if (StateMachineComponent->GetState() == EState::ES_Dead)
			{
				if (HealthStatComponent->GetCurrentValue() > 0.0f)
				{
					StateMachineComponent->SetState(EState::ES_Stand);
				}
			}

			return StateMachineComponent->GetState() == EState::ES_Stand || StateMachineComponent->GetState() == EState::ES_Stun;
		}
	}
	else
	{
		return false;
	}
}

bool ACharacterBase::CanSprint()
{
	return (StateMachineComponent->GetState() == EState::ES_Stand);
}


void ACharacterBase::ResetStatus()
{
	StateMachineComponent->SetState(GetCharacterMovement()->IsFalling() ? EState::ES_Jump : EState::ES_Stand);
}

void ACharacterBase::ResetCombat(bool ResetStatus)
{
	AttackType = EAttackType::EAT_Invalid;

	InputBufferComponent->bIsOpen = false;

	if (ResetStatus)
	{
		this->ResetStatus();
	}
	else
	{
	}
}

void ACharacterBase::ResetJump()
{
	if (StateMachineComponent->GetState() == EState::ES_Jump)
	{
		ResetStatus();
	}
}


void ACharacterBase::ResetRootmotion()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		bIsWeaponToggling = false;
	}
}

void ACharacterBase::ResetRootmotionMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		bIsWeaponToggling = false;
	}
}

void ACharacterBase::IgnoreRootmotion()
{
	EnableRootmotionTimerHandle.Invalidate();

	bIsRootmotionIgnored = true;


	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		GetWorldTimerManager().SetTimer(EnableRootmotionTimerHandle, this, &ACharacterBase::EnableRootmotion, 1.2f, false);
	}
}

void ACharacterBase::EnableRootmotion()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}

	bIsRootmotionIgnored = false;

}

void ACharacterBase::OrientToController()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ACharacterBase::OrientToMovement()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ACharacterBase::OnInputBufferConsumed(EInputBufferKey InputKey)
{
	switch (InputKey)
	{
	case EInputBufferKey::EIBK_LightAttack:
	case EInputBufferKey::EIBK_HeavyAttack:
	case EInputBufferKey::EIBK_SpecialAttack1:
	case EInputBufferKey::EIBK_SpecialAttack2:
	case EInputBufferKey::EIBK_SpecialAttack3:
	case EInputBufferKey::EIBK_SpecialAttack4:
	{
		MainAttack(InputKey);
	} break;
	case EInputBufferKey::EIBK_ToggleStance:
		ToggleWeapon();
		break;
	case EInputBufferKey::EIBK_Defense:
		Defense();
		break;
	case EInputBufferKey::EIBK_Crouch:
		Crouch();
		break;
	case EInputBufferKey::EIBK_Interact:
		InteractWith();
		break;
	case EInputBufferKey::EIBK_JumpOrEvade:
	{
		if (StanceState == EStanceState::ESS_Unarmed)
		{
			DoJump();
		}
		else
		{
			Evade(GetEvadeDirection());
		}
	} break;
	case EInputBufferKey::EIBK_ToggleWalkRun :
	{
		ToggleWalkRun();
	}break;
	case EInputBufferKey::EIBK_StartSprint: 
	{
		StartSprint();
	}break;
	case EInputBufferKey::EIBK_StopSprint : 
	{
		StopSprint();
	}break;
	default :
	{
		AWeaponBase* EquippedWeapon = EquipmentComponent->FindWeaponActor();
		if (EquippedWeapon)
		{
			EquippedWeapon->ResetCombo();
		}
	} break;
	}
}

void ACharacterBase::OnInputBufferClose()
{
	if (StateMachineComponent->GetState() == EState::ES_Stun)
	{
		ResetCombat(false);
	}
	else
	{
		ResetCombat(true);
	}
}

void ACharacterBase::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && 
		(OtherActor != this) && 
		(InteractionComponent != nullptr))
	{
		InteractionComponent->AddInteractable(OtherActor);
	}
}

void ACharacterBase::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && 
		(OtherActor != this) && 
		(InteractionComponent != nullptr))
	{
		InteractionComponent->RemoveInteractable(OtherActor);
	}
}

void ACharacterBase::OnWeaponHit_Implementation(const FHitResult& HitResult)
{
	if (IAttackable* Attackable = Cast<IAttackable>(HitResult.GetActor()))
	{
		AWeaponBase* Weapon = EquipmentComponent->FindWeaponActor();
		if (Attackable->IsVulnerable() && Weapon != nullptr)
		{
			IAttackable::Execute_HandleHit(HitResult.GetActor(), HitResult, this, Weapon->GetAttribute());
			OnHitSuccess(HitResult);
		}
	}
}

void ACharacterBase::OnHitSuccess_Implementation(const FHitResult& HitResult)
{
}

void ACharacterBase::OnDefenseSuccess_Implementation(AActor* Attacker)
{
	FVector BlockSocketLocation;
	AWeaponBase* Weapon = EquipmentComponent->FindWeaponActor();
	BlockSocketLocation = (Weapon == nullptr) ? Weapon->GetActorLocation() : Weapon->Mesh->GetSocketLocation("Block_0");
	if (Weapon != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetParryParticle(), FTransform(BlockSocketLocation));
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Weapon->GetParrySound(), BlockSocketLocation);
		if (ACharacterBase * CastedHitActor = Cast<ACharacterBase>(Attacker))
		{
			CastedHitActor->Stun(CastedHitActor->GetStunDirection(GetActorLocation()), true);
		}
	}
}

void ACharacterBase::OnBeingHitted_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage)
{
	//FRotator RotationZ = UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal);
	FVector Direction = (HitResult.TraceStart - HitResult.TraceEnd).GetSafeNormal();
	FRotator RotationEffect = UKismetMathLibrary::MakeRotFromZ(Direction);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, FTransform(RotationEffect, HitResult.ImpactPoint));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, FTransform(RotationEffect, HitResult.ImpactPoint));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	AccumulatedDamage += Damage;
	if (AccumulatedDamage > DamageAccumulationLimit)
	{
		Stun(GetStunDirection(Attacker->GetActorLocation()), true);
		ResetAccumulatedDamage();
		ResetAccumulatedDamageTimerHandle.Invalidate();
	}
	else
	{
		Stun(GetStunDirection(Attacker->GetActorLocation()), false);
		if (ResetAccumulatedDamageTimerHandle.IsValid())
		{
			ResetAccumulatedDamageTimerHandle.Invalidate();
		}
		GetWorldTimerManager().SetTimer(ResetAccumulatedDamageTimerHandle, this, &ACharacterBase::ResetAccumulatedDamage, 3.0f, false);
	}
	HealthStatComponent->SubStat(Damage);
}


FVector ACharacterBase::GetEvadeDirection()
{
	return GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal();
}

EMovementDirection ACharacterBase::GetStunDirection(FVector HitterLocation)
{
	FVector ToHitterDirection = HitterLocation - GetActorLocation();
	ToHitterDirection.Normalize();
	float DotForwardAndHitterDirection = FVector::DotProduct(ToHitterDirection, GetActorForwardVector());

	float AbsDot = FMath::Abs(DotForwardAndHitterDirection);

	if (AbsDot > 0.5f)
	{
		if (FMath::Sign(DotForwardAndHitterDirection) > 0.0f)
		{
			return EMovementDirection::EMD_Forward;
		}
		else
		{
			return EMovementDirection::EMD_Backward;
		}
	}
	else
	{
		FVector LocalHitterLocation = GetActorTransform().InverseTransformPosition(HitterLocation);
		if (LocalHitterLocation.Y < 0.0f)
		{
			return EMovementDirection::EMD_Left;
		}
		else
		{
			return EMovementDirection::EMD_Right;
		}
	}
}

void ACharacterBase::OnStateEnter(EState State)
{
}

void ACharacterBase::OnStateEnd(EState State)
{
}
