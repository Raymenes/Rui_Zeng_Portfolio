//
//  GameMode.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/2/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Actor.h"
#include <vector>
#include "Tile.hpp"
#include "PathNode.h"
#include "HUD.hpp"
#include "GameTimers.h"

class GameMode : public Actor
{
    DECL_ACTOR(GameMode, Actor);
    
public:
    GameMode(Game& game);
    
    void BeginPlay() override;
    
    void SelectTile();
        
    void SpawnEnemyWave();
    
    void SpawnSingleEnemy();
    
    void BuildCannonTower();
    
    void BuildFrostTower();
    
    void GetMoney(){
        mMoney += income;
        UpdateHUD();
    }
    
    void LoseHealth(){
        mHealth -= damage;
        if(mHealth<0){
            mHealth = 0;
            UpdateMessage("Game Over");
        }
        UpdateHUD();
    }
    
    void UpdateHUD();
    
    void UpdateMessage(std::string message);
    
    void ClearMessage();

private:
    
    HUDPtr mHealthHUD;
    HUDPtr mMoneyHUD;
    HUDPtr mStatusHUD;
    
    TimerHandle messageTimerHandle;
    
    TilePtr mTileMap[9][18];
    TilePtr mChosenTile;
    
    NavWorldPtr mNavWorld;
    
    int baseEnemyNum;
    int waveNum;
    
    int income;
    int damage;
    int mHealth;
    int mMoney;
    int frostTowerCost;
    int cannonTowerCost;
};

DECL_PTR(GameMode)