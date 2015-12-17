// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "Tetromino.h"
#include "TetrisManager.h"
#include "TCube.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATetromino::ATetromino()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    //init cubes map
    mCubesVec.resize(3);
    for (int i = 0; i < 3; ++i) {
        mCubesVec[i].resize(4);
        for(int j =0; j < 4; ++j){
            mCubesVec[i][j] = nullptr;
        }
    }
}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
    Super::BeginPlay();
    
}

void ATetromino::Initialize(const Tetris_Direction &dir, const Tetris_Shape& shape, const TetrisCoord coord)
{
    mMovingDir = dir;
    mShape = shape;
    mTetrominoCoord = coord;
    
    //Set Size
    float GridScale = ATetrisManager::get()->GetGridScale();
    SetActorScale3D(FVector(GridScale, GridScale, GridScale));
    
    //Spawn the actual tetromino consisting of TCubes
    SpawnAllChildCubes();
    
    GetWorldTimerManager().SetTimer(MoveTimer, this, &ATetromino::Move, 1.0f, true);
    
    manager = ATetrisManager::get();
    if(manager)
    {
        manager->RegisterTetromino(this);
    }
}

// Called every frame
void ATetromino::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
    
}

//temp debug function
void ATetromino::Rotate()
{
    if(fixed) return;
    
    Tetris_Direction nextDir = Tetris_Direction::T_Left;
    if(mFacingDir == Tetris_Direction::T_Left)
    {
        nextDir = Tetris_Direction::T_Forward;
    }
    else if(mFacingDir == Tetris_Direction::T_Right)
    {
        nextDir = Tetris_Direction::T_Backward;
    }
    else if(mFacingDir == Tetris_Direction::T_Forward)
    {
        nextDir = Tetris_Direction::T_Right;
    }
    else if(mFacingDir == Tetris_Direction::T_Backward)
    {
        nextDir = Tetris_Direction::T_Left;
    }
    Rotate(nextDir);
}

//this is not relative rotation to the current quaternion
//but instead, is snapping to the fixed facing direction
void ATetromino::Rotate(const Tetris_Direction& dir)
{
    if(fixed) return;
    
    
    // Check if the next rotation grid is empty and valid so we can do this
    if(CanRotate(dir))
    {
        mFacingDir = dir;
        TetrisCoord RotateVec;
        FVector origin = FVector::ZeroVector;
        TetrisCoord temp[4];
        TetrisCoord old[4];
        int index = 0;
        for (int i = 0; i < 3; ++i)
        {
            for(int j =0; j < 4; ++j)
            {
                if(mCubesVec[i][j])
                {
                    int x = i;
                    int y = j;
                    ATCube* cube = mCubesVec[i][j];
                    
                    if(dir == Tetris_Direction::T_Left)
                    {
                        RotateVec.SetCoord(3-y, x, 0);
                    }
                    else if(dir == Tetris_Direction::T_Right)
                    {
                        RotateVec.SetCoord(y, 2-x, 0);
                    }
                    else if(dir == Tetris_Direction::T_Forward)
                    {
                        RotateVec.SetCoord(x, y, 0);
                    }
                    else if(dir == Tetris_Direction::T_Backward)
                    {
                        RotateVec.SetCoord(2-x, 3-y, 0);
                    }
                    //Update Manager Box Map
                    ATetrisManager::get()->ChangeCubeRegistration(cube->GetCoord(), mTetrominoCoord + RotateVec, cube);
                    temp[index] = RotateVec + mTetrominoCoord;
                    old[index] = cube->GetCoord();
                    index++;
                    
                    //Update Cube Coord
                    cube->SetCoord(mTetrominoCoord + RotateVec);
                    //                    ATetrisManager::get()->RegisterCube(cube->GetCoord(), cube);
                    
                    //the actual changing physical location in the game world
                    float GridLength = ATetrisManager::get()->GetGridLength();
                    mCubesVec[i][j]->SetActorRelativeLocation(origin
                                                              + FVector::RightVector * GridLength * RotateVec.x
                                                              + FVector::ForwardVector * GridLength * RotateVec.y
                                                              + FVector::UpVector * GridLength * RotateVec.z);
                }
            }
        }
        ATetrisManager::get()->RemoveTetrominoOnRadar(old);
        ATetrisManager::get()->SetTetrominoOnRadar(temp);
    }
    else
    {
//        Debug_Log("Cannot Rotate!");
        //TODO: Handle the situation when its not empty, maybe connect with other cubes
    }
}

bool ATetromino::CanRotate(const Tetris_Direction& facing)
{
    if(fixed) return false;
    
    mFacingDir = facing;
    TetrisCoord RotateVec;
    FVector origin = FVector::ZeroVector;
    
    for (int i = 0; i < 3; ++i)
    {
        for(int j =0; j < 4; ++j)
        {
            if(mCubesVec[i][j])
            {
                int x = i;
                int y = j;
                
                if(facing == Tetris_Direction::T_Left)
                {
                    RotateVec.SetCoord(3-y, x, 0);
                }
                else if(facing == Tetris_Direction::T_Right)
                {
                    RotateVec.SetCoord(y, 2-x, 0);
                }
                else if(facing == Tetris_Direction::T_Forward)
                {
                    RotateVec.SetCoord(x, y, 0);
                }
                else if(facing == Tetris_Direction::T_Backward)
                {
                    RotateVec.SetCoord(2-x, 3-y, 0);
                }
                TetrisCoord nextGrid = mTetrominoCoord + RotateVec;
                // Check if the next rotation grid is empty so we can do this
                // TODO, just checking empty is not enough, since the occupying block could be moved to the other place
                //the grid is not valid
                if (! ATetrisManager::get()->IsValidGrid(nextGrid))
                {
//                    Debug_Log("Next Is Not Valid Grid!");
                    return false;
                }
                else
                {
                    //the grid is not empty
                    if (! ATetrisManager::get()->IsGridEmpty(nextGrid))
                    {
                        ATCube* cube = ATetrisManager::get()->GetCubeAtGrid(nextGrid);
                        if(cube)
                        {
                            if(cube->mParent != this)
                                return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void ATetromino::Move()
{
    if(fixed) return;
    
    MoveHelper(mMovingDir);
}

void ATetromino::SingleMoveAtDirection(const Tetris_Direction& dir)
{
    MoveHelper(dir);
}

void ATetromino::ChangeMovingDirection(const Tetris_Direction& dir)
{
    mMovingDir = dir;
}

void ATetromino::MoveHelper(const Tetris_Direction& dir)
{
    if(fixed) return;
    
    TetrisCoord moveVec;
    
    if(dir == Tetris_Direction::T_Left){
        moveVec.SetCoord(-1, 0, 0);
    }
    else if(dir == Tetris_Direction::T_Right){
        moveVec.SetCoord(1, 0, 0);
    }
    else if(dir == Tetris_Direction::T_Forward){
        moveVec.SetCoord(0, 1, 0);
    }
    else if(dir == Tetris_Direction::T_Backward){
        moveVec.SetCoord(0, -1, 0);
    }
    else if(dir == Tetris_Direction::T_Upward){
        moveVec.SetCoord(0, 0, 1);
    }
    else if(dir == Tetris_Direction::T_Downward){
        moveVec.SetCoord(0, 0, -1);
    }
    
    if(CanMove(moveVec))
    {
        mTetrominoCoord = mTetrominoCoord + moveVec;
        SetActorLocation(ATetrisManager::get()->GridToWorldPosition(mTetrominoCoord));
        TetrisCoord temp[4];
        TetrisCoord old[4];
        int index = 0;
        for (int i = 0; i < 3; ++i) {
            for(int j =0; j < 4; ++j){
                if(mCubesVec[i][j])
                {
                    ATCube* cube = mCubesVec[i][j];
                    ATetrisManager::get()->ChangeCubeRegistration(cube->GetCoord(), moveVec + cube->GetCoord(), cube);
                    temp[index] = moveVec + cube->GetCoord();
                    old[index] = cube->GetCoord();
                    index++;
                    mCubesVec[i][j]->SetCoord(moveVec + cube->GetCoord());
                    
                }
            }
        }
        ATetrisManager::get()->RemoveTetrominoOnRadar(old);
        ATetrisManager::get()->SetTetrominoOnRadar(temp);
        
        
    }
    else
    {
//        Debug_Log("Cannot Move!");
        //TODO: Handle the situation when its not empty, maybe connect with other cubes
    }
}

bool ATetromino::CanMove(const TetrisCoord& moveVec)
{
    if(fixed) return false;
    
    for (int i = 0; i < 3; ++i) {
        for(int j =0; j < 4; ++j){
            if(mCubesVec[i][j]){
                TetrisCoord nextGrid;
                
                nextGrid = mCubesVec[i][j]->GetCoord() + moveVec;
                //the grid is not valid
                if (! ATetrisManager::get()->IsValidGrid(nextGrid))
                {
//                    Debug_Log("Next Is Not Valid Grid!");
                    
                    if(mCubesVec[i][j]->GetCoord().z == 0)
                    {
                        FixTetromino();
                    }
                    else if(mMovingDir != Tetris_Direction::T_Downward)
                    {
                        mMovingDir = Tetris_Direction::T_Downward;
                    }

                    
                    return false;
                }
                else
                {
                    //this block may have bug

                    //the grid is valid but not empty (occupied by other cubes)
                    if (! ATetrisManager::get()->IsGridEmpty(nextGrid))
                    {
                        
                        ATCube* otherCube = ATetrisManager::get()->GetCubeAtGrid(nextGrid);
                        if(otherCube != nullptr)
                        {
                            if (otherCube->mParent != this) {
                                FixTetromino();
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

void ATetromino::BeginDestroy()
{
    Super::BeginDestroy();
    if(manager)
    {
        manager->DeregisterTetromino(this);
    }
}

//not using this for the moment, may delete in the future
void ATetromino::SetManager(class ATetrisManager *m)
{
    manager = m;
    if(manager)
    {
        manager->RegisterTetromino(this);
    }
}

void ATetromino::SetShape(Tetris_Shape shape = Tetris_Shape::T_L1)
{
    mShape = shape;
}

void ATetromino::SpawnAllChildCubes()
{
    Tetris_Shape shape = mShape;
    if(shape == Tetris_Shape::T_L1)
    {
        SpawnChildCube(0,0);
        SpawnChildCube(0,1);
        SpawnChildCube(0,2);
        SpawnChildCube(1,0);
    }
    else if(shape == Tetris_Shape::T_L2)
    {
        SpawnChildCube(0,0);
        SpawnChildCube(1,1);
        SpawnChildCube(1,2);
        SpawnChildCube(1,0);
    }
    else if(shape == Tetris_Shape::T_Z1)
    {
        SpawnChildCube(0,1);
        SpawnChildCube(1,1);
        SpawnChildCube(2,0);
        SpawnChildCube(1,0);
    }
    else if(shape == Tetris_Shape::T_Z2)
    {
        SpawnChildCube(0,0);
        SpawnChildCube(1,1);
        SpawnChildCube(2,1);
        SpawnChildCube(1,0);
    }
    else if(shape == Tetris_Shape::T_I)
    {
        SpawnChildCube(0,0);
        SpawnChildCube(0,1);
        SpawnChildCube(0,2);
        SpawnChildCube(0,3);
    }
    else if(shape == Tetris_Shape::T_T)
    {
        SpawnChildCube(0,0);
        SpawnChildCube(1,0);
        SpawnChildCube(2,0);
        SpawnChildCube(1,1);
    }
    
}

void ATetromino::SpawnChildCube(int xCoord, int yCoord)
{
    if (xCoord<0 || xCoord >2 || yCoord < 0 || yCoord >3) {
        //invalid cube position
        return;
    }
    if(mCubesVec[xCoord][yCoord] != nullptr){
        //already have a cube at the position
        return;
    }
    
    float GridLength = ATetrisManager::get()->GetGridLength();
    
    FVector offSet = FVector::ForwardVector * GridLength * yCoord
    + FVector::RightVector * GridLength * xCoord;
    FVector SpawnLocation = GetActorLocation() + offSet;
    FRotator SpawnRotation = GetActorRotation();
    AActor* Char = GetWorld()->SpawnActor<AActor>(CubeClass, offSet, SpawnRotation);
    if(Char){
        ATCube* cube = Cast<ATCube>(Char);
        if(cube){
            mCubes.Add(cube);
            
            mCubesVec[xCoord][yCoord] = cube;
            cube->AttachRootComponentToActor(this);
            //make sure the world coord is set for each individual cube when spawned
            cube->SetCoord(mTetrominoCoord.x + xCoord, mTetrominoCoord.y + yCoord, mTetrominoCoord.z);
            //should set parent tetromino
            cube->mParent = this;
            cube->Initialize(mShape);
            ATetrisManager::get()->RegisterCube(cube->GetCoord(), cube);
            
        }
    }
}


void ATetromino::FixTetromino ()
{
    fixed = true;
    if(ATetrisManager::get()->mSelectedTetromino == this)
    {
        ATetrisManager::get()->mSelectedTetromino = nullptr;
    }
    
    for (int i = 0; i < 3; ++i)
    {
        for(int j =0; j < 4; ++j)
        {
            if(mCubesVec[i][j])
            {
                ATCube* cube = mCubesVec[i][j];
                ATetrisManager::get()->FixCube(cube);
                
                cube->DetachRootComponentFromParent(true);
                cube->mParent = nullptr;
                Destroy();
            }
        }
    }
}


void ATetromino::Repaint()
{
    for (int i = 0; i < 3; ++i)
    {
        for(int j =0; j < 4; ++j)
        {
            if(mCubesVec[i][j])
            {
                ATCube* cube = mCubesVec[i][j];
                cube->Repaint();
            }
        }
    }
}

void ATetromino::Highlight()
{
    for (int i = 0; i < 3; ++i)
    {
        for(int j =0; j < 4; ++j)
        {
            if(mCubesVec[i][j])
            {
                ATCube* cube = mCubesVec[i][j];
                cube->Highlight();
            }
        }
    }
}

void ATetromino::ClockwiseRotate()
{
    if(fixed) return;
    
    Tetris_Direction nextDir = Tetris_Direction::T_Left;
    if(mFacingDir == Tetris_Direction::T_Left)
    {
        nextDir = Tetris_Direction::T_Forward;
    }
    else if(mFacingDir == Tetris_Direction::T_Right)
    {
        nextDir = Tetris_Direction::T_Backward;
    }
    else if(mFacingDir == Tetris_Direction::T_Forward)
    {
        nextDir = Tetris_Direction::T_Right;
    }
    else if(mFacingDir == Tetris_Direction::T_Backward)
    {
        nextDir = Tetris_Direction::T_Left;
    }
    Rotate(nextDir);
}

void ATetromino::CounterClockwiseRotate()
{
    if(fixed) return;
    
    Tetris_Direction nextDir = Tetris_Direction::T_Left;
    if(mFacingDir == Tetris_Direction::T_Left)
    {
        nextDir = Tetris_Direction::T_Backward;
    }
    else if(mFacingDir == Tetris_Direction::T_Right)
    {
        nextDir = Tetris_Direction::T_Forward;
    }
    else if(mFacingDir == Tetris_Direction::T_Forward)
    {
        nextDir = Tetris_Direction::T_Left;
    }
    else if(mFacingDir == Tetris_Direction::T_Backward)
    {
        nextDir = Tetris_Direction::T_Right;
    }
    Rotate(nextDir);
}