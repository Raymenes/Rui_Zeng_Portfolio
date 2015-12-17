// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include <string>
#include "Weapon.h"
#include "TetrisClimberHUD.generated.h"

//#include "TetrisClimberCharacter.h"
//#include <vector>
/**
 * 
 */
UCLASS()
class TETRISCLIMBER_API ATetrisClimberHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    
    ATetrisClimberHUD();
    virtual void DrawHUD() override;
	void Get3DCrossHairPos();
    void SetFixedTetromino(int32 x, int32 y);
    void RemoveFixedTetromino(int32 x, int32 y);
    void SetMovingTetromino(int32 x, int32 y);
    void RemoveMovingTetromino(int32 x, int32 y);
    void SetPlayer(ACharacter* Player) {character = Player;}
    
    FVector2D brc;
	FVector2D CrossHairCenter;
	FVector CrossHair3DPos;
	float mAimDistance = 10000.0f;

	bool mDrawPauseMenu = true;
	bool mGameOver = false;
	bool mLost = false;
	AWeapon::ProjectileType mCurrentProjType = AWeapon::ProjectileType::UP;
private:
    /** Crosshair asset pointer */
    class UTexture2D* CrosshairTex;
    class UTexture2D* FixedTetrominoTex;
    class UTexture2D* radarTex;
    class UTexture2D* greenTex;
    class UTexture2D* blackTex;
    class UTexture2D* greenRedTex;
    class UTexture2D* blueCircleTex;
    class ATetrisClimberCharacter* player;
    ACharacter* character;
    //int32*
    //std::vector< std::vector<bool> > fixed;
    bool fixed[10][10];
    bool moving[10][10];
protected:
	UFont* mFont;
	//std::string mRulesString;
};

