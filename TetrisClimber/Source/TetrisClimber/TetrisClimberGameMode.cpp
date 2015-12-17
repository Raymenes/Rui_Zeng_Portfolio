// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TetrisClimber.h"
#include "TetrisClimberGameMode.h"
#include "TetrisClimberCharacter.h"
#include "PlayBoundary.h"
#include "TetrisManager.h"
#include "FlyingEnemy.h"
#include "TetrisClimberHUD.h"

ATetrisClimberGameMode::ATetrisClimberGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
  	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/FirstPersonCPP/Blueprints/MyTetrisClimberCharacter2"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
    DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
    
    //HUDClass = ::StaticClass();
    HUDClass = ATetrisClimberHUD::StaticClass();
    
}


void ATetrisClimberGameMode::BeginPlay(){
    
    Super::BeginPlay();
    
    ATetrisManager* tm = ATetrisManager::get();
    tm->Initialize();
    
    int32 width = tm->GetBoxWidth(); //X
    int32 height = tm->GetBoxHeight(); //Z
    int32 length = tm->GetBoxLength(); //Y
    FVector location = tm->GetBoxOrigin();

    UWorld* const world = GetWorld();
    APlayBoundary* tile;
    FRotator rotator = FRotator(0, 0, 0);
    FVector next;
    FVector extents;
    FVector irrelevant;

	//adding binding for pausing/unpausing and reset
	world->GetFirstPlayerController()->InputComponent->BindAction("TogglePause", IE_Pressed, this, &ATetrisClimberGameMode::OnTogglePause);
	world->GetFirstPlayerController()->InputComponent->BindAction("ToggleReset", IE_Pressed, this, &ATetrisClimberGameMode::OnToggleReset);

    tile = world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass());
    FBox box = tile->GetComponentsBoundingBox(false);
    box.GetCenterAndExtents(irrelevant, extents);
    FVector offset = FVector(0, 2*extents.Y, 2*extents.Z);
    location -= offset;
    
    negativeZbound = location.Z;
    negativeYbound = location.Y;
    negativeXbound = location.X;
    positiveXbound = location.X + (width * extents.X);
    positiveYbound = location.Y + (length * extents.Y);
    positiveZbound = location.Z + (height * extents.Z);
    
    if(world){
        
        for(int i = 0; i < width; i++){
            
            for(int j = 0; j < length; j++){
            
            box = tile->GetComponentsBoundingBox(false);
            FVector center;
            box.GetCenterAndExtents(center, extents);
            next = FVector(0, extents.Y, 0);
            location += 2*next;
            world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            
            }
        
        location = tm->GetBoxOrigin() - offset;
        next = FVector(extents.X, 0, 0);
        location += (i+1) * 2* next;
        }
        
        //setup wall one (height and width)
        location = tm->GetBoxOrigin() - offset;
        next = FVector(0, 0, extents.Z);
        location += 2*next;
        FVector wall = location;
        tile = world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
        
        for(int i = 0; i < height; i++){
            
            for(int j = 0; j < width - 1; j++){
                
                box = tile->GetComponentsBoundingBox(false);
                FVector center;
                box.GetCenterAndExtents(center, extents);
                next = FVector(extents.X, 0, 0);
                location += 2*next;
                world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            }
            
            location = wall;
            next = FVector(0, 0, extents.Z);
            location += (i+1) * 2 * next;
            world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            
       }
        
        //setup wall two (height and length)
        location = tm->GetBoxOrigin() - offset;
        next = FVector(-1*extents.Y, 0, extents.Z);
        location += 2*next;
        wall = location;
        tile = world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
        
        for(int i = 0; i < height; i++){//height
            
            location = wall;
            next = FVector(0, 0, extents.Z);
            location += (i) * 2 * next;
            world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            
            for(int j = 0; j < length; j++){//length
                
                box = tile->GetComponentsBoundingBox(false);
                FVector center;
                box.GetCenterAndExtents(center, extents);
                next = FVector(0, extents.Y, 0);
                location += 2*next;
                world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            }
            
        }
        
        
        //setup wall 3
        wall = FVector(location.X, location.Y, tm->GetBoxOrigin().Z - offset.Z);
        next = FVector(extents.X, extents.Y, extents.Z);
        location = wall;
        location += 2*next;
        wall = location;
        tile = world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
        
        for(int i = 0; i < height; i++){//height
            location = wall;
            next = FVector(0, 0, extents.Z);
            location += (i) * 2 * next;
            world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            for(int j = 0; j < width - 1; j++){//width
                
                box = tile->GetComponentsBoundingBox(false);
                FVector center;
                box.GetCenterAndExtents(center, extents);
                next = FVector(extents.X, 0, 0);
                location += 2*next;
                world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
                
            }
            
        }
        
        
        
        //setup wall 4
        wall = FVector(location.X, location.Y, tm->GetBoxOrigin().Z - offset.Z);
        next = FVector(extents.X, -1*extents.Y, extents.Z);
        location = wall;
        location += 2*next;
        wall = location;
        tile = world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
        
        for(int i = 0; i < height; i++){
            location = wall;
            next = FVector(0, 0, extents.Z);
            location += (i) * 2 * next;
            world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
            for(int j = 0; j < length; j++){
                
                box = tile->GetComponentsBoundingBox(false);
                FVector center;
                box.GetCenterAndExtents(center, extents);
                next = FVector(0, extents.Y, 0);
                location -= 2*next;
                world->SpawnActor<APlayBoundary>(APlayBoundary::StaticClass(), location, rotator);
                
            }
            
            
        }
        
        //world->SpawnActor<AFlyingEnemy>(AFlyingEnemy::StaticClass());
        //world->SpawnActor<AFlyingEnemy>();
        
        
    }
	//Start the game off paused so player can read instructions for a bit
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	FInputActionBinding& toggle = GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("TogglePause", IE_Pressed, this, &ATetrisClimberGameMode::OnTogglePause);
	toggle.bExecuteWhenPaused = true; //EVEN THOUGH THE GAME IS PAUSED, CATCH THIS EVENT !!!!
    
}

void ATetrisClimberGameMode::OnTogglePause()
{
	mPaused = !mPaused;
	Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->mDrawPauseMenu = !(Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->mDrawPauseMenu);
	if (mPaused)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	FInputActionBinding& toggle = GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("TogglePause", IE_Pressed, this, &ATetrisClimberGameMode::OnTogglePause);
	toggle.bExecuteWhenPaused = true; //EVEN THOUGH THE GAME IS PAUSED, CATCH THIS EVENT !!!!
}

void ATetrisClimberGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//check if game is over or nah
	auto HUD = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (HUD->mGameOver)
	{
		auto character = Cast<ATetrisClimberCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
		if (character)
		{
			mPaused = true;
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			FInputActionBinding& toggle = GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("ToggleReset", IE_Pressed, this, &ATetrisClimberGameMode::OnToggleReset);
			toggle.bExecuteWhenPaused = true; //EVEN THOUGH THE GAME IS PAUSED, CATCH THIS EVENT !!!!
			
			
			if (!character->IsAlive())//if character died then end the game
			{
				HUD->mLost = true;
			}
			/*else//we won
			{
				HUD->mLost = false;
			}*/
		}
	}
}

void ATetrisClimberGameMode::OnToggleReset()//reset the game..hope this works
{
	/*auto HUD = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (HUD->mGameOver)
	{
		//auto HUD = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		auto character = Cast<ATetrisClimberCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
		character->SetIsAlive(true);
		HUD->mGameOver = false;
		HUD->mLost = false;
		mPaused = false;
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		ATetrisManager* tm = ATetrisManager::get();
		tm->Reset();
		tm->ResetMap();		
		character->ResetLocation();
	}*/
}
