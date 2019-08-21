
#include "Public/Actors/Players/MyPlayerController.h"
#include "Public/Actors/Players/PlayerCharacter.h"
#include "Public/Widgets/InteractionWidget.h"

#include "Public/Widgets/MainHUD.h"
#include "Public/Widgets/ConversationWidget.h"

AMyPlayerController::AMyPlayerController()
{
}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());

	PlayerCharacterRef = Cast<APlayerCharacter>(GetPawn());
	check(PlayerCharacterRef != nullptr);

	MainHUDRef = (UMainHUD *)CreateWidget<UMainHUD>(this, MainHUDClass, FName("MainHud"));
	MainHUDRef->AddToViewport();
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPlayerController::ShowInteractionWidget(const FText& MessageToShow)
{
	MainHUDRef->ShowInteractionMessage(MessageToShow);
}

void AMyPlayerController::HideInteractionWidget()
{
	MainHUDRef->HideInteractionMessage();
}

void AMyPlayerController::CreateConversationWidget()
{
	ConversationWidgetRef = (UConversationWidget*)CreateWidget<UConversationWidget>(this, ConversationWidgetClass, FName("ConversationWidget"));
	ConversationWidgetRef->AddToViewport(10);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ConversationWidgetRef->TakeWidget());
	SetInputMode(InputMode);

	OnConversationBegin.Broadcast();
}

void AMyPlayerController::DestroyConversationWidget()
{
	ConversationWidgetRef->RemoveFromParent();
	
	SetInputMode(FInputModeGameOnly());

	OnConversationEnd.Broadcast();
}

void AMyPlayerController::OnDead_Implementation()
{
}

