// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FlyingEnemy.generated.h"

UCLASS()
class TETRISCLIMBER_API AFlyingEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlyingEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
    
    void NotifyActorBeginOverlap(AActor* OtherActor) override;

    
    float GetDirection() { return direction; }
    FVector GetLocation() { return enemyLocation; }
    FVector GetForwardVector() { return forwardVector; }
    float GetSpeed() { return speed; }
    
private:
    float direction;
    FVector enemyLocation;
    FVector forwardVector;
    float speed;
    
    
};
