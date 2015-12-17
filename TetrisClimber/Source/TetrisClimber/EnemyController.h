// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class TETRISCLIMBER_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
    void BeginPlay() override;
    void Tick(float deltaTime) override;
	
};
