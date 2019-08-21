// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/PickupBase.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Public/ASGameInstance.h"

#include "Public/Actors/Players/MyPlayerController.h"
#include "Public/Components/InventoryComponent.h"


#define LOCTEXT_NAMESPACE "Pickup"

APickupBase::APickupBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetupAttachment(RootComponent);

	CollisionBoxComponent->SetCollisionProfileName("PickUp");
	CollisionBoxComponent->SetSimulatePhysics(true);

	ID = "1";
	MessageToShow = FText::FromString("Press E To Pickup");
}


void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	if (UASGameInstance * Instance = Cast<UASGameInstance>(GetGameInstance()))
	{
		ItemData = Instance->GetItemDB()->FindRow<FItemData>(GetID(), "");
		check(ItemData != nullptr);
	}

}

void APickupBase::Interact_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
	UInventoryComponent* InventoryComponent = (UInventoryComponent*)Actor->GetComponentByClass(UInventoryComponent::StaticClass());
	if (InventoryComponent != nullptr)
	{
		InventoryComponent->AddItem(GetClass(), 1);

		Destroy();
	}
}

void APickupBase::ShowFeedbackWidget_Implementation(AActor* Actor)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (AMyPlayerController * MyPC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("Message"), MessageToShow);
			Arguments.Add(TEXT("Name"), ItemData->Name);
			MyPC->ShowInteractionWidget(FText::Format(LOCTEXT("FeedbackText", "{Message} {Name}"), Arguments));
		}
	}
}

void APickupBase::HideFeedbackWidget_Implementation(AActor* Actor)
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

//void APickupBase::Drop(AActor* Actor)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Pickp Base Drop"));
//}
