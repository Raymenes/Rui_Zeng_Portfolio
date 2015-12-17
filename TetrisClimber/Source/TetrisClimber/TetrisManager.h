// Fill out your copyright notice in the Description page of Project Settings.
// By Rui Zeng, 2015/11/30, All Rights Reserved

#pragma once
#include <vector>
#include "Tetromino.h"
#include "TCube.h"
#include "Singleton.h"
#include "TetrisClimberHUD.h"
#include "GameFramework/Actor.h"
#include "TetrisManager.generated.h"

UCLASS()
class TETRISCLIMBER_API ATetrisManager : public AActor, public Singleton<ATetrisManager>
{
    
    GENERATED_BODY()
    DECLARE_SINGLETON(ATetrisManager)
    
public:
    // Sets default values for this actor's properties
    ATetrisManager();
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;
    
    /*
     Get Called by GameMode at BeginPlay() to set up timers
     to spawn tetromono and clear layers
     */
    void Initialize();
    
    /*
     * TODO: (currently just empty placeholder)
     */
    void RegisterTetromino(ATetromino* piece){}
    
    void DeregisterTetromino(ATetromino* piece){}
    
    /*
     * Check if the grid is empty, return false is there is already a cube inside the grid
     * Also return false is the grid is not valid
     */
    bool IsGridEmpty(const TetrisCoord& coord);
    
    /*
     * Check if the coord is a valid grid inside the 3D map
     */
    bool IsValidGrid(const TetrisCoord& coord);
    
    /*
     * Register and Deregister functions will check if cube pointer valid and if grid valid
     */
    void RegisterCube(const TetrisCoord& coord, ATCube* cube)
    {
        if (cube) {
            if(IsValidGrid(coord)){
                mTetrisCubes[coord.z][coord.y][coord.x] = cube;
                if(hud){
                    hud->SetMovingTetromino(coord.x, coord.y);
                }
                
            }
        }
    }
    
    void ChangeCubeRegistration(const TetrisCoord& OldCoord, const TetrisCoord& NewCoord, ATCube* cube)
    {
        if (cube) {
            if(IsValidGrid(OldCoord) && IsValidGrid(NewCoord))
            {
                mTetrisCubes[NewCoord.z][NewCoord.y][NewCoord.x] = cube;
                mTetrisCubes[OldCoord.z][OldCoord.y][OldCoord.x] = nullptr;
                if(hud && (OldCoord.x != NewCoord.x || OldCoord.y != NewCoord.y)){
                    //hud->RemoveMovingTetromino(OldCoord.x, OldCoord.y);
                    //hud->SetMovingTetromino(NewCoord.x, NewCoord.y);
                    
                }
            }
        }
    }
    
    void UpdateCubeOnRadar(const TetrisCoord& NewCoord){
        if(IsValidGrid(NewCoord) && hud){
            hud->SetMovingTetromino(NewCoord.x, NewCoord.y);
        }
    }
    
    void RemoveAndSetCube(const TetrisCoord& OldCoord, const TetrisCoord& NewCoord){
        hud->RemoveMovingTetromino(OldCoord.x, OldCoord.y);
        hud->SetMovingTetromino(NewCoord.x, NewCoord.y);
    }
    
    void RemoveTetrominoOnRadar(TetrisCoord* old){
        
        for(int i = 0; i < 4; i++){
            hud->RemoveMovingTetromino(old[i].x, old[i].y);
        }
    }
    
    void SetTetrominoOnRadar(TetrisCoord* newT){
        
        for(int i = 0; i < 4; i++){
            hud->SetMovingTetromino(newT[i].x, newT[i].y);
        }
        
    }
    
    /*
     * Doesn't check if this is a valid grid
     * return the world position of the center of the grid in FVector
     */
    FVector GridToWorldPosition(const TetrisCoord& coord);
    
    /*
     * return the pointer to the TCube at the grid
     * return nullptr if no TCube at grid or grid is not valid
     */
    ATCube* GetCubeAtGrid(const TetrisCoord& coord);
    
    /*
     * Given a world position, returns the corresponding grid coord
     * Doesn't check if the world/grid coord is valid
     */
    TetrisCoord WorldToGridPosition(const FVector& position);
    FVector2D WorldTo2DGridPosition(const FVector& position);
    
    /*
     * return true if this is a valid coord to spawn the tetromino of specified shape
     */
    bool CanSpawn(const TetrisCoord& coord, Tetris_Shape shape);
    
    //TODO, called by a regular looping timer
    void KeepSpawnRandomTetromino();
    
    /*
     * performs the actual spawning, should be called after the CanSpawn Check
     */
    void SpawnTetromino(const TetrisCoord& coord, Tetris_Shape shape);
    
    //Mostly For Debug
    void SpawnCube(const TetrisCoord& coord);
    
    void ResetMap();
    
    //return true if the layer is full
    //return false if the layer is not, or the layerIndex is invalid
    bool IsLayerFull(int layerIndex);
    
    void ClearLayer(int layerIndex);
    
    void ClearBottomLayer(){ClearLayer(0);};
    
    bool IsRowFull(int layerIndex, int rowIndex);
    
    bool IsColumnFull(int layerIndex, int colIndex);
    
    void ClearRow(int layerIndex, int rowIndex);
    
    void ClearColunm(int layerIndex, int colIndex);
    
    //move the cube down till there is no empty grid to occupy
    void FixCube(ATCube* cube);
    
    int32 GetBoxWidth() { return BoxWidth; }
    int32 GetBoxLength() { return BoxLength; }
    int32 GetBoxHeight() { return BoxHeight; }
    FVector GetBoxOrigin() { return BoxOrigin; }
    float GetGridScale(){ return GridScale; }
    float GetGridLength(){return GridLength; }
    float GetActualGridLength(){ return GridScale * GridLength; };
    
    void InitializeHUD(ATetrisClimberHUD* Hud);
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    float GridScale = 2.0f;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    TArray<ATetromino*> mTetrominos;
    
    
    bool IsShootToSelect(){return ShootToSelect;}
    
    int getScore(){return mScore;}
    
    class ATetromino* mSelectedTetromino = nullptr;
    
protected:
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    int32 BoxWidth = 10;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    int32 BoxLength = 10;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    int32 BoxHeight = 15;
    
    int32 mScore = 0;
    
    float GridLength = 100.0f;
    
    FTimerHandle SpawnTimer;
    
    FTimerHandle ClearTimer;
    
    ATetrisClimberHUD* hud;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    bool ShootToSelect = true;
        
    UPROPERTY(EditAnywhere, Category = Tetromino)
    float SpawnInterval = 5.0f;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    float ClearInterval = 10.0f;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    FVector BoxOrigin = FVector(250.0f, -440.0f, 700.0f); // [center world position] of the front bottom left 1st grid in the box
    //should be manually set in the editor
    
    //Should be hooked to MyTetromino Blueprinted class, manually set in the editor
    UPROPERTY(EditAnywhere, Category = Tetromino)
    TSubclassOf<AActor> TetrominoClass;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    TSubclassOf<AActor> CubeClass;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    TArray<FTetrisCoord> SpawnCoords;

    std::vector< std::vector< std::vector<ATCube*> > > mTetrisCubes;
};
