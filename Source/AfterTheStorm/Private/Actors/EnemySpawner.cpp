// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/EnemySpawner.h"
#include "Engine/World.h"

#include "Public/Actors/NPCs/NPCBase.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	//UWorld* World = GetWorld();
	//if (World != nullptr)
	//{
	//	World->SpawnActor<ANPCBase>()
	//}
}
