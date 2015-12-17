// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "EnemyController.h"
#include "FlyingEnemy.h"


void AEnemyController::BeginPlay(){
    
    Super::BeginPlay();
    //AActor* pawn = Cast<AActor>(GetPawn());
    //MoveToLocation(pawn->GetActorLocation() + FVector(0, 10000, 0));

    
    
}


void AEnemyController::Tick(float deltaTime){
    
    
    Super::Tick(deltaTime);
    
    
    
    AFlyingEnemy* pawn = Cast<AFlyingEnemy>(GetPawn());
    FVector speed = FVector(0, 150, 0);
    //float speed = pawn->GetSpeed();
    float actorDirection = pawn->GetDirection();
    //FVector forwardVector = pawn->GetForwardVector();
    
    //UMovementComponent moveComp = pawn->GetComponentByClass(TSubclassOf<UActorComponent>UCharacterMovementComponent);
    speed *= deltaTime * actorDirection;
    pawn->SetActorLocation(pawn->GetActorLocation() + speed);
    
    
    
    
    
}