// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConversationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UConversationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConversationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public :
	UFUNCTION(BlueprintCallable, Category = "Conversation")
	void StartConversation();

	UFUNCTION(BlueprintCallable, Category = "Conversation")
	void FinishConversation();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	class UBehaviorTree* ConversationTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	class UBlackboardData* ConversationBlackboard;


	UPROPERTY()
	class UBehaviorTree* PrevTree;
	UPROPERTY()
	class UBlackboardData* PrevBlackboard;
};
