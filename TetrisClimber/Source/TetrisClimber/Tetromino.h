// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "TetrisStatistics.h"
#include "GameFramework/Actor.h"
#include "Tetromino.generated.h"



UCLASS()
class TETRISCLIMBER_API ATetromino : public AActor
{
    GENERATED_BODY()
    
public:
    
    // Sets default values for this actor's properties
    ATetromino();
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;
    
    void Initialize(const Tetris_Direction& dir, const Tetris_Shape& shape, const TetrisCoord coord);
    
    void Rotate(const Tetris_Direction& dir);
    
    //Move the Tetromino by only 1 unit in the given direction
    //leave for player controll
    void SingleMoveAtDirection(const Tetris_Direction& dir);
    
    void Move();
    
    void ChangeMovingDirection(const Tetris_Direction& dir);
    
    virtual void BeginDestroy() override;
    
    //get called right after being spawned;
    void SetManager(class ATetrisManager* m);
    
    void SetShape(Tetris_Shape shape);
    
    void SpawnAllChildCubes();
    
    void SpawnChildCube(int xCoord, int yCoord);
    
    void SetCoord(int x, int y, int z)
    {
        mTetrominoCoord.x = x;
        mTetrominoCoord.y = y;
        mTetrominoCoord.z = z;
    }
    
    void SetCoord(TetrisCoord coord)
    {
        mTetrominoCoord = coord;
    }
    
    const TetrisCoord& GetCoord(){return mTetrominoCoord;}
    
    //TODO: Break the tetromino down into individual cubes and make then fixed
    void FixTetromino ();
    
    void Repaint();
    
    void Highlight();
    
    void ClockwiseRotate();
    
    void CounterClockwiseRotate();
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    bool fixed = false;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    TArray<class ATCube*> mCubes;
    
protected:
    //temp debug func
    void Rotate();
    
    void MoveHelper(const Tetris_Direction& dir);
    
    bool CanMove(const TetrisCoord& moveVec);
    
    bool CanRotate(const Tetris_Direction& facing);
    
    FTimerHandle MoveTimer;
    
    std::vector <std::vector<class ATCube*> > mCubesVec;
    
    Tetris_Direction mFacingDir;
    Tetris_Direction mMovingDir;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    FTetrisCoord mTetrominoCoord; /*the (0,0,0) local origin coord for each tetromino piece is at the bottom left*/
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    float MoveSpeed = 1.0f;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    class ATetrisManager* manager;
    
    Tetris_Shape mShape;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    TSubclassOf<AActor> CubeClass;
    
};
