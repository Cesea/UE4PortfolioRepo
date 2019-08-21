// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/*
 */
UCLASS()
class AFTERTHESTORM_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public :
	AMyPlayerController();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//class URotationControlComponent* RotationControlComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConversationBeginEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConversationEndEvent);

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FConversationBeginEvent OnConversationBegin;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FConversationEndEvent OnConversationEnd;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void ShowInteractionWidget(const FText& MessageToShow);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void HideInteractionWidget();

	UFUNCTION(BlueprintCallable, Category = "Conversation")
	void CreateConversationWidget();

	UFUNCTION(BlueprintCallable, Category = "Conversation")
	void DestroyConversationWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDead();
	virtual void OnDead_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleOptions();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleShops(AActor* OtherActor, bool MoneyTrade);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleInventory();


protected:
	//virtual void SetupInputComponent() override;

public :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class UMainHUD> MainHUDClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	class UMainHUD* MainHUDRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class UConversationWidget> ConversationWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	class UConversationWidget* ConversationWidgetRef;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerCharacter* PlayerCharacterRef;
};
