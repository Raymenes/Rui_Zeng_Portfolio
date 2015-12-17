// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "TetrisManager.h"
#include "PlayBoundary.h"
#include <iostream>
#include <stdlib.h>

#define Debug_Log(i) \
std::cout << i << std::endl;\


// Sets default values
ATetrisManager::ATetrisManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    if(_instance == nullptr){
        _instance = this;
    }else{
        //        Destroy();
    }
    
    ResetMap();
}

// Called when the game starts or when spawned
void ATetrisManager::BeginPlay()
{
    Super::BeginPlay();
    
    //actual legit spawning timer
    KeepSpawnRandomTetromino();
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATetrisManager::KeepSpawnRandomTetromino, SpawnInterval, true);
    
//    GetWorldTimerManager().SetTimer(ClearTimer, this, &ATetrisManager::ClearBottomLayer, ClearInterval, true);

}

void ATetrisManager::Initialize()
{
   ResetMap();
}

// Called every frame
void ATetrisManager::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

bool ATetrisManager::IsGridEmpty(const TetrisCoord& coord)
{
    if(!IsValidGrid(coord)) {
        return false;
    }
    if (mTetrisCubes[coord.z][coord.y][coord.x] != nullptr) {
        return false;
    }
    return (mTetrisCubes[coord.z][coord.y][coord.x] == nullptr);
}

bool ATetrisManager::IsValidGrid(const TetrisCoord& coord)
{
    return (coord.x >= 0 && coord.y >= 0 && coord.z >= 0
            && coord.x < BoxWidth && coord.y < BoxLength && coord.z < BoxHeight);
}

ATCube* ATetrisManager::GetCubeAtGrid(const TetrisCoord& coord)
{
    if(IsGridEmpty(coord)) return nullptr;
    
    return mTetrisCubes[coord.z][coord.y][coord.x];
}

FVector ATetrisManager::GridToWorldPosition(const TetrisCoord& coord)
{
    return (BoxOrigin + FVector(coord.y * GridLength*GridScale, coord.x * GridLength*GridScale, coord.z * GridLength*GridScale));
}

TetrisCoord ATetrisManager::WorldToGridPosition(const FVector& position)
{
    FVector offset = position - BoxOrigin;
    float length = GridLength*GridScale;
    return TetrisCoord(static_cast<int>(offset.X/length),
                       static_cast<int>(offset.Y/length),
                       static_cast<int>(offset.Z/length));
}

FVector2D ATetrisManager::WorldTo2DGridPosition(const FVector& position){
    
    FVector offset = position - BoxOrigin;
    float length = GridLength*GridScale;
    return FVector2D(offset.Y/length, offset.X/length);

}

void ATetrisManager::KeepSpawnRandomTetromino()
{
    Tetris_Shape shape = Tetris_Shape::T_L1;
    int shapeID = rand()%6;
    if (shapeID == 0)
    {
        shape = Tetris_Shape::T_L1;
    }
    else if(shapeID == 1)
    {
        shape = Tetris_Shape::T_L2;
    }
    else if(shapeID == 2)
    {
        shape = Tetris_Shape::T_Z1;
    }
    else if(shapeID == 3)
    {
        shape = Tetris_Shape::T_Z2;
    }
    else if(shapeID == 4)
    {
        shape = Tetris_Shape::T_I;
    }
    else if(shapeID == 5)
    {
        shape = Tetris_Shape::T_T;
    }
    
    bool canSpawn = false;
    TetrisCoord randomCoord(BoxLength/2,BoxWidth/2,BoxHeight-1);
    while(!canSpawn)
    {
        randomCoord.SetCoord(rand()%BoxLength, rand()%BoxWidth, BoxHeight-1);
        canSpawn = CanSpawn(randomCoord, shape);
    }
    if(CanSpawn(randomCoord, shape))
    {
        SpawnTetromino(randomCoord, shape);
    }
    else
        std::cout<< "Cannot Spawn" << std::endl;
}

bool ATetrisManager::CanSpawn(const TetrisCoord& coord, Tetris_Shape shape)
{
    if(shape == Tetris_Shape::T_L1)
    {
        if(IsGridEmpty(coord) &&
           IsGridEmpty(coord + TetrisCoord(0,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(0,2,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,0,0))
           )
        {
            return true;
        }
    }
    else if(shape == Tetris_Shape::T_L2)
    {
        if(IsGridEmpty(coord) &&
           IsGridEmpty(coord + TetrisCoord(1,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,2,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,0,0))
           )
        {
            return true;
        }
    }
    else if(shape == Tetris_Shape::T_Z1)
    {
        if(IsGridEmpty(coord + TetrisCoord(0,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(2,0,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,0,0))
           )
        {
            return true;
        }
    }
    else if(shape == Tetris_Shape::T_Z2)
    {
        if(IsGridEmpty(coord) &&
           IsGridEmpty(coord + TetrisCoord(1,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(2,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,0,0))
           )
        {
            return true;
        }
    }
    else if(shape == Tetris_Shape::T_I)
    {
        if(IsGridEmpty(coord) &&
           IsGridEmpty(coord + TetrisCoord(0,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(0,2,0)) &&
           IsGridEmpty(coord + TetrisCoord(0,3,0))
           )
        {
            return true;
        }
    }
    else if(shape == Tetris_Shape::T_T)
    {
        if(IsGridEmpty(coord) &&
           IsGridEmpty(coord + TetrisCoord(1,0,0)) &&
           IsGridEmpty(coord + TetrisCoord(2,1,0)) &&
           IsGridEmpty(coord + TetrisCoord(1,1,0))
           )
        {
            return true;
        }
    }
    return false;
}

void ATetrisManager::SpawnTetromino(const TetrisCoord& coord, Tetris_Shape shape)
{
    
    AActor* Char = GetWorld()->SpawnActor<AActor>(TetrominoClass, GridToWorldPosition(coord), FRotator(0.0f, 0.0f, 0.0f));
    if(Char)
    {
        ATetromino* t = Cast<ATetromino>(Char);
        if (t) {
            t->Initialize(Tetris_Direction::T_Downward, shape, coord);
            mTetrominos.Add(t);
        }
    }
}

bool ATetrisManager::IsLayerFull(int layerIndex)
{
    if(layerIndex < 0 || layerIndex >= BoxHeight) return false;
    
    for (int i = 0; i < BoxLength; ++i)
    {
        for (int j = 0; j < BoxWidth; ++j)
        {
            if (mTetrisCubes[layerIndex][i][j] == nullptr)
            {
                return false;
            }
        }
    }
    return true;
}

bool ATetrisManager::IsRowFull(int layerIndex, int rowIndex)
{
    if(layerIndex < 0 || layerIndex >= BoxHeight) return false;
    if(rowIndex < 0 || rowIndex >= BoxLength) return false;
    for (int j = 0; j < BoxWidth; ++j)
    {
        if (mTetrisCubes[layerIndex][rowIndex][j] == nullptr)
            return false;
        
    }
    return true;
}

bool ATetrisManager::IsColumnFull(int layerIndex, int colIndex)
{
    if(layerIndex < 0 || layerIndex >= BoxHeight) return false;
    if(colIndex < 0 || colIndex >= BoxWidth) return false;
    for (int i = 0; i < BoxLength; ++i)
    {
        if (mTetrisCubes[layerIndex][i][colIndex] == nullptr)
            return false;
        
    }
    return true;
}
//Y
void ATetrisManager::ClearRow(int layerIndex, int rowIndex)
{
    if(layerIndex < 0 || layerIndex >= BoxHeight) return ;
    if(rowIndex < 0 || rowIndex >= BoxLength) return ;
    for (int j = 0; j < BoxWidth; ++j)
    {
        ATCube* cube = mTetrisCubes[layerIndex][rowIndex][j];
        if (cube)
        {
            mTetrisCubes[layerIndex][rowIndex][j] = nullptr;
            cube->Destroy();
            mScore += 10;
            
            for (int k = layerIndex + 1; k < BoxHeight; ++k)
            {
                ATCube* upperCube = mTetrisCubes[k][rowIndex][j];
                if(upperCube)
                {
                    if (upperCube->IsFixed)
                    {
                        TetrisCoord nextGrid = TetrisCoord(j,rowIndex,k-1);
                        ChangeCubeRegistration(upperCube->GetCoord(), nextGrid, upperCube);
                        upperCube->SetCoord(nextGrid);
                        upperCube->SetActorLocation(GridToWorldPosition(nextGrid));
                    }
                }
                else {
                    hud->RemoveFixedTetromino(j, rowIndex);
                    
                }
            }
        }
        
    }
}
//X
//Doesn't check if the colunm is full
void ATetrisManager::ClearColunm(int layerIndex, int colIndex)
{
    if(layerIndex < 0 || layerIndex >= BoxHeight) return;
    if(colIndex < 0 || colIndex >= BoxWidth) return;
    
    
    for (int i = 0; i < BoxLength; ++i)
    {
        ATCube* cube = mTetrisCubes[layerIndex][i][colIndex];
        if (cube)
        {
            mTetrisCubes[layerIndex][i][colIndex] = nullptr;
            cube->Destroy();
            mScore += 10;
            
            for (int k = layerIndex + 1; k < BoxHeight; ++k)
            {
                ATCube* upperCube = mTetrisCubes[k][i][colIndex];
                if(upperCube)
                {
                    if (upperCube->IsFixed)
                    {
                        TetrisCoord nextGrid = TetrisCoord(colIndex,i,k-1);
                        ChangeCubeRegistration(upperCube->GetCoord(), nextGrid, upperCube);
                        upperCube->SetCoord(nextGrid);
                        upperCube->SetActorLocation(GridToWorldPosition(nextGrid));
                    }
                }
                else hud->RemoveFixedTetromino(colIndex, i);
            }
        }
        //perform full layer check for each layer
        //    for(int i = 0; i < BoxHeight; ++i)
        //    {
        //        if(IsLayerFull(i))
        //        {
        //            ClearLayer(i);
        //        }
        //    }
        
    }
}

//buggy, cause the game to crash
//hud has bad access
void ATetrisManager::ClearLayer(int layerIndex)
{
    Debug_Log("Clear Layer");
    for (int i = 0; i < BoxLength; ++i)
    {
        for(int j = 0; j < BoxWidth; ++j)
        {
            ATCube* cube = mTetrisCubes[layerIndex][i][j];
            if (cube)
            {
                mTetrisCubes[layerIndex][i][j] = nullptr;
                cube->Destroy();
            }
            
            for (int k = layerIndex + 1; k < BoxHeight; ++k)
            {
                ATCube* upperCube = mTetrisCubes[k][i][j];
                if(upperCube)
                {
                    if (upperCube->IsFixed)
                    {
                        TetrisCoord nextGrid = TetrisCoord(j,i,k-1);
                        ChangeCubeRegistration(upperCube->GetCoord(), nextGrid, upperCube);
                        upperCube->SetCoord(nextGrid);
                        upperCube->SetActorLocation(GridToWorldPosition(nextGrid));
                    }
                }
            }
            if (mTetrisCubes[0][i][j] == nullptr)
            {
                hud->RemoveFixedTetromino(j, i);
            }
        }
    }
}

void ATetrisManager::FixCube(ATCube* cube)
{
    if(cube->IsHighlighted)
    {
        cube->Repaint();
    }
    
    TetrisCoord nextGrid = cube->GetCoord();
    TetrisCoord tempNext = nextGrid + TetrisCoord(0,0,-1);
    
    if ( IsGridEmpty(tempNext)) {
        nextGrid = tempNext;
        ChangeCubeRegistration(cube->GetCoord(), nextGrid, cube);
        cube->SetCoord(nextGrid);
        cube->SetActorLocation(GridToWorldPosition(nextGrid));
        if(hud){
            hud->RemoveMovingTetromino(cube->GetCoord().x, cube->GetCoord().y);
            hud->SetMovingTetromino(nextGrid.x, nextGrid.y);
        }
        FixCube(cube);
        
    }else{
        cube->IsFixed = true;
        int x = cube->GetCoord().x;
        int y = cube->GetCoord().y;
        int z = cube->GetCoord().z;
        bool colFull = IsColumnFull(z, x);
        bool rowFull = IsRowFull(z, y);
        if (rowFull)
        {
            Debug_Log("Clear Row!");
            ClearRow(z, y);
        }
        if (colFull)
        {
            Debug_Log("Clear Colunm!");
            ClearColunm(z, x);
        }
        //perform full layer check for each layer
        //    for(int i = 0; i < BoxHeight; ++i)
        //    {
        //        if(IsLayerFull(i))
        //        {
        //            ClearLayer(i);
        //        }
        //    }
        /*ATetrisClimberHUD* hud = Cast(GetWorld()->GetFirstPlayerController()->GetHUD());
        hud->SetFixedTetromino(cube->GetCoord().x, cube->GetCoord().y);*/
        //UWorld* world = GetWorld();
        //APlayerController* pc = world->GetFirstPlayerController();
        //if(pc){
        //    ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
            //hud->SetFixedTetromino(cube->GetCoord().x, cube->GetCoord().y);
        //}
        //ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(world->GetFirstPlayerController()->GetHUD());
        hud->SetFixedTetromino(cube->GetCoord().x, cube->GetCoord().y);
        hud->RemoveMovingTetromino(cube->GetCoord().x, cube->GetCoord().y);
        return;
    }
    
}


void ATetrisManager::InitializeHUD(ATetrisClimberHUD* Hud)
{
    hud = Hud;
}

void ATetrisManager::ResetMap()
{
    mScore = 0;
//
//    for (int i = 0; i < BoxHeight; ++i)
//    {
//        for(int j = 0; j < BoxLength; ++j)
//        {
//            for (int k = 0; k < BoxWidth; ++k)
//            {
//                ATCube* cube = mTetrisCubes[i][j][k];
//                if (cube)
//                {
//                    cube->Destroy();
//                    mTetrisCubes[i][j][k] = nullptr;
//                }
//            }
//        }
//    }
    mTetrisCubes.clear();
    mTetrisCubes.resize(BoxHeight, std::vector<std::vector<ATCube*>>());
    for (size_t i = 0; i < BoxHeight; ++i)
    {
        mTetrisCubes[i].resize(BoxLength, std::vector<ATCube*>());
        for (size_t j = 0; j < BoxLength; ++j)
        {
            mTetrisCubes[i][j].resize(BoxWidth, nullptr);
        }
    }
}

void ATetrisManager::SpawnCube(const TetrisCoord& coord)
{
    UWorld* const world = GetWorld();
    if(world)
    {
        AActor* Char = world->SpawnActor<ATCube>(CubeClass, GridToWorldPosition(coord), FRotator(0,0,0));
        
        ATCube* cube = Cast<ATCube>(Char);
        if(cube)
        {
            cube->SetActorScale3D(FVector(GridScale, GridScale, GridScale));
        }
    }
}

