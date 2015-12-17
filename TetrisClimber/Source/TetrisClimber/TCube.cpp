// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "TCube.h"


// Sets default values
ATCube::ATCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATCube::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATCube::Initialize(Tetris_Shape shape)
{
    TArray<UStaticMeshComponent*> Components;
    GetComponents<UStaticMeshComponent>(Components);
    
    UStaticMeshComponent* StaticMeshComponent = nullptr;//added by Alan, if it breaks anything I'll remove it.
    for( int32 i=0; i<Components.Num(); i++ )
    {
        mMeshComponent = Components[i];
    }
    if(shape == Tetris_Shape::T_I)
    {
        mMaterial = TheMaterial;
        mMeshComponent->SetMaterial(0, TheMaterial);
    }
    else if (shape == Tetris_Shape::T_L1 || shape == Tetris_Shape::T_L2)
    {
        mMaterial = TheRightMaterial;
        mMeshComponent->SetMaterial(0, TheRightMaterial);
    }
    else if (shape == Tetris_Shape::T_Z1 || shape == Tetris_Shape::T_Z2)
    {
        mMaterial = TheLeftMaterial;
        mMeshComponent->SetMaterial(0, TheLeftMaterial);
    }
    else if (shape == Tetris_Shape::T_T)
    {
        mMaterial = TheMaterial;
        mMeshComponent->SetMaterial(0, TheMaterial);
    }
}

void ATCube::Repaint()
{
    mMeshComponent->SetMaterial(0, mMaterial);
    IsHighlighted = false;
}

void ATCube::Highlight()
{
    mMeshComponent->SetMaterial(0, TheHighlighMaterial);
    IsHighlighted = true;
}

