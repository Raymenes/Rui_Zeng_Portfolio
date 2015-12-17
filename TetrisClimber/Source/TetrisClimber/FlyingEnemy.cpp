// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "FlyingEnemy.h"
#include "EnemyController.h"
#include <stdlib.h>
#include <time.h>
#include "TetrisClimberGameMode.h"
#include "PlayBoundary.h"
#include "Tetromino.h"
#include "TCube.h"

// Sets default values
AFlyingEnemy::AFlyingEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    AIControllerClass = AEnemyController::StaticClass();
    
    //static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/tilecube3'"));
    //tileMesh->SetStaticMesh(StaticMesh.Object);
    
    
    ///Game/ThirdPersonCPP/BluePrints/BP_FlyingEnemy
    ///Game/Mannequin/Character/Mesh/FSM
    static ConstructorHelpers::FObjectFinder<USkeletalMesh>skelMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/FSM'"));
    GetMesh()->SetSkeletalMesh(skelMesh.Object);
    
    
    srand(time(NULL));
    float random;
    
    
    direction = 1;
    if(GetWorld()){
        ATetrisClimberGameMode* gameMode = GetWorld()->GetAuthGameMode<ATetrisClimberGameMode>();
        
        if(gameMode){
        
        int minWidth = gameMode->GetNegativeXBound();
        int maxWidth = gameMode->GetPositiveXBound();
        int widthDifference = maxWidth - minWidth;
        //random = static_cast<float>( rand()) /( static_cast<float>(RAND_MAX/( widthDifference + 1)));
        random = rand() % widthDifference + 1;
        float xValue = minWidth + random;
        
        int minLength = gameMode->GetNegativeYBound();
        int maxLength = gameMode->GetPositiveYBound();
        int lengthDifference = maxLength - minLength;
        //random = static_cast<float>( rand()) /( static_cast<float>(RAND_MAX/( lengthDifference + 1)));
        random = rand() % lengthDifference + 1;
        float yValue = minLength + random;
        
        int minHeight = gameMode->GetNegativeZBound();
        int maxHeight = gameMode->GetPositiveZBound();
        int heightDifference = maxHeight - minHeight;
        //random = static_cast<float>( rand() )/( static_cast<float>(RAND_MAX/( heightDifference + 1)));
        random = rand() % heightDifference + 1;
        float zValue = minHeight + random;
  
        
        enemyLocation = FVector(xValue, yValue, zValue);
        
        forwardVector = FVector(0, 250, 0);
        speed = 1;
        SetActorLocation(enemyLocation);
        FRotator rotator = FRotator(0, 0, 0);
        //GetWorld()->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), enemyLocation, rotator);
        FVector scale = FVector(1.0f);
        GetMesh()->SetWorldScale3D(scale * 30.0f);
        }
    }

    
}

// Called when the game starts or when spawned
void AFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();

    FVector scale = FVector(1.0f);
    GetMesh()->SetWorldScale3D(scale * 30.0f);
    

}

// Called every frame
void AFlyingEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    float speed1 = speed;
    float actorDirection = direction;
    FVector forwardVector1 = forwardVector;
    
    //UMovementComponent moveComp = pawn->GetComponentByClass(TSubclassOf<UActorComponent>UCharacterMovementComponent);
    forwardVector1 *= DeltaTime * actorDirection * speed1;
    SetActorLocation(GetActorLocation() + forwardVector1);
    
    
}

// Called to bind functionality to input
void AFlyingEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AFlyingEnemy::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
    
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    
    //direction *= -1;
    
}



void AFlyingEnemy::NotifyActorBeginOverlap(AActor* OtherActor){
    
    
    Super::NotifyActorBeginOverlap(OtherActor);
    
    ATCube* tetromino = Cast<ATCube>(OtherActor);
    //Tetris_Direction direction = "Right";
    if(tetromino){
        //tetromino->SingleMoveAtDirection(Tetris_Direction::T_Right);
    }
    else
    direction *= -1;
    
    
}


