// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/InteractableBase.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBoxComponent->SetCollisionProfileName("WorldDynamic");
	RootComponent = CollisionBoxComponent;

	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphereComponent->SetupAttachment(RootComponent);

	InteractionSphereComponent->SetCollisionProfileName("Interaction");

	MessageToShow = FText::FromString("Press E To Interact");
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::Interact_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
}

void AInteractableBase::ShowFeedbackWidget_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Show Feedback"));
}

void AInteractableBase::HideFeedbackWidget_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Hide Feedback"));
}


void AInteractableBase::EnableCollisions()
{
	CollisionBoxComponent->SetSimulatePhysics(true);
	CollisionBoxComponent->SetCollisionProfileName("PickUp");
	InteractionSphereComponent->SetCollisionProfileName("Interaction");
}

void AInteractableBase::DisableCollisions()
{
	CollisionBoxComponent->SetSimulatePhysics(false);
	CollisionBoxComponent->SetCollisionProfileName("NoCollision");
	InteractionSphereComponent->SetCollisionProfileName("NoCollision");
}

