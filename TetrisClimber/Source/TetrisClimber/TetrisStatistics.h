//
//  TetrisStatistics.h
//  TetrisClimber
//
//  Created by Rui Zeng on 11/26/15.
//  Copyright © 2015 EpicGames. All rights reserved.
//

#pragma once
#include <iostream>
#include "TetrisStatistics.generated.h"

#define Debug_Log(i) \
std::cout << i << std::endl;\

UENUM(BlueprintType)
enum class Tetris_Shape : uint8
{
    T_L1    UMETA(DisplayName = "L_Shape_1"),
    T_L2    UMETA(DisplayName = "L_Shape_2"),
    T_Z1    UMETA(DisplayName = "Z_Shape_1"),
    T_Z2    UMETA(DisplayName = "Z_Shape_2"),
    T_I    UMETA(DisplayName = "I_Shape"),
    T_T    UMETA(DisplayName = "T_Shape"),
};

UENUM(BlueprintType)
enum class Tetris_Direction : uint8
{
    T_Left      UMETA(DisplayName = "Left"),
    T_Right     UMETA(DisplayName = "Right"),
    T_Forward   UMETA(DisplayName = "Forward"),
    T_Backward  UMETA(DisplayName = "Backward"),
    T_Upward    UMETA(DisplayName = "Upward"),
    T_Downward    UMETA(DisplayName = "Downward"),
};


UENUM(BlueprintType)
enum class Projectile_Action : uint8
{
    T_Rotation    UMETA(DisplayName = "Rotate"),
    T_Movement    UMETA(DisplayName = "Move"),
};

USTRUCT(BlueprintType)
struct FTetrisCoord
{
    GENERATED_USTRUCT_BODY()
//    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, Category = Tetromino)
    int32 x;
    UPROPERTY(EditAnywhere, Category = Tetromino)
    int32 y;
    UPROPERTY(EditAnywhere, Category = Tetromino)
    int32 z;
    
    FTetrisCoord():x(0),y(0),z(0){}
    
    FTetrisCoord(int inX, int inY, int inZ):x(inX),y(inY),z(inZ){}
    
    void SetCoord(int inX, int inY, int inZ)
    {
        x = inX;
        y = inY;
        z = inZ;
    }
    
    FTetrisCoord operator+(const FTetrisCoord& rhs) const
    {
        FTetrisCoord ret(x + rhs.x, y + rhs.y, z + rhs.z);
        return ret;
    }
    
    FTetrisCoord operator-(const FTetrisCoord& rhs) const
    {
        FTetrisCoord ret(x - rhs.x, y - rhs.y, z - rhs.z);
        return ret;
    }
    
    FTetrisCoord operator*(int mul) const
    {
        FTetrisCoord ret(x * mul, y * mul, z * mul);
        return ret;
    }
    
    bool operator==(const FTetrisCoord& rhs) const
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }
    
    bool operator!=(const FTetrisCoord& rhs) const
    {
        return (x != rhs.x || y != rhs.y || z != rhs.z);
    }
};

typedef FTetrisCoord TetrisCoord;