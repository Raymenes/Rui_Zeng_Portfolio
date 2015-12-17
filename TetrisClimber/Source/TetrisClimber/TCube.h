// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TetrisStatistics.h"
#include "GameFramework/Actor.h"
#include "TCube.generated.h"


UCLASS()
class TETRISCLIMBER_API ATCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATCube();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void SetCubeSize(float size){SetActorScale3D(FVector(size, size, size));}
    
    void Initialize(Tetris_Shape shape);
    
    void Repaint();
    
    void Highlight();
    
    void SetCoord(int x, int y, int z) 
    {
        mCubeCoord.x = x;
        mCubeCoord.y = y;
        mCubeCoord.z = z;
    }
    
    void SetCoord(TetrisCoord coord)
    {
        mCubeCoord = coord;
    }
    
    TetrisCoord GetCoord(){return mCubeCoord;}
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    class ATetromino* mParent;
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    bool IsFixed = false;
    
    bool IsHighlighted = false;
    
protected:
    UStaticMeshComponent* mMeshComponent;
    UMaterialInstance* mMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    UMaterialInstance* TheMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    UMaterialInstance* TheRightMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    UMaterialInstance* TheDownMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    UMaterialInstance* TheLeftMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    UMaterialInstance* TheHighlighMaterial;
private:
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    FTetrisCoord mCubeCoord;
    
};

