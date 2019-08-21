// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConversationWidget.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UConversationWidget : public UUserWidget
{
	GENERATED_BODY()

public :

protected :

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation", Meta = (ExposeOnSpawn = "true"))
	class ANPCBase* NPCTalkWith;
	
};
