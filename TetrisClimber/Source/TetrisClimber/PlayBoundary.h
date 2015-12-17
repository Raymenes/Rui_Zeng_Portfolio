// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlayBoundary.generated.h"

UCLASS()
class TETRISCLIMBER_API APlayBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayBoundary();
    APlayBoundary(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    void DrawGrid();
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
    
    void NotifyActorBeginOverlap(AActor* OtherActor) override;

    void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
    
    
    
protected:
    UPROPERTY(VisibleDefaultsOnly, Category = Tile)
    UBoxComponent* CollisionComp;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Tile)
    UStaticMeshComponent* tileMesh;
    
    //TSubclassOf<APlayBoundary> tileBP;
    
	
};
