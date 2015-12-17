 // Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "PlayBoundary.h"
#include "TetrisClimberCharacter.h"

// Sets default values
APlayBoundary::APlayBoundary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
}

APlayBoundary::APlayBoundary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){
	
    CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComp"));
    
    tileMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TileMesh"));
    tileMesh->SetMobility(EComponentMobility::Movable);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/tilecube3'"));
    tileMesh->SetStaticMesh(StaticMesh.Object);
    
    
    RootComponent = tileMesh;
    CollisionComp->AttachTo(RootComponent);
    
}

// Called when the game starts or when spawned
void APlayBoundary::BeginPlay()
{
	Super::BeginPlay();
    DrawGrid();
}


void APlayBoundary::DrawGrid(){
    
    
}


// Called every frame
void APlayBoundary::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APlayBoundary::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit){


    


}

//used when the actor overlaps the boundary
void APlayBoundary::NotifyActorBeginOverlap(AActor* OtherActor){
    
    ATetrisClimberCharacter* player = Cast<ATetrisClimberCharacter>(OtherActor);
    
    if(player){
        
    }

    
    
    
}
