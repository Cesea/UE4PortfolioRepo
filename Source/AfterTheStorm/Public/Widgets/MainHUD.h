// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public :

protected :

public :
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MainHUD")
	void StartDialog();
	virtual void StartDialog_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MainHUD")
	void FinishDialog();
	virtual void FinishDialog_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractionText")
	void ShowInteractionMessage(const FText& Message);
	virtual void ShowInteractionMessage_Implementation(const FText& Message);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractionText")
	void HideInteractionMessage();
	virtual void HideInteractionMessage_Implementation();


};
