//
//  GameMode.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/2/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "GameMode.hpp"
#include "Game.h"
#include "Enemy.hpp"
#include "CannonTower.hpp"
#include "PathNode.h"
#include <iostream>
#include <string>
#include <sstream>

//helper functions
std::string NumberToString ( int Number )
{
    std::stringstream ss;
    ss << Number;
    return ss.str();
}

IMPL_ACTOR(GameMode, Actor);

GameMode::GameMode(Game& game)
: Actor(game), baseEnemyNum(4), waveNum(1),
    mHealth(100), mMoney(120),
    frostTowerCost(25), cannonTowerCost(20),
    damage(10), income(10)
{}

void GameMode:: BeginPlay(){
    //initialize gameObjects here
    Vector3 initPos(-425.0f, 200.0f, 0.0f);
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 18; ++j) {
            auto tile = Tile::Spawn(mGame);
            tile->SetPosition(initPos);
            
            mTileMap[i][j] = tile;
            initPos.operator+=(Vector3(50.0f, 0.0f, 0.0f));
        }
        initPos.Set(-425.0f, initPos.y-50.0f, 0.0f);
    }
    //initialize enemy and player base
    {
        //enemy spawning tile
        mTileMap[3][0]->SwitchTile(2);
        mTileMap[3][0]->isSpecial = true;
        mTileMap[4][0]->SwitchTile(2);
        mTileMap[4][0]->isSpecial = true;
        mTileMap[5][0]->SwitchTile(2);
        mTileMap[5][0]->isSpecial = true;
        mTileMap[4][1]->SwitchTile(2);
        mTileMap[4][1]->isSpecial = true;
        //base tile
        mTileMap[4][17]->SwitchTile(3);
        mTileMap[4][17]->isSpecial = true;
        mTileMap[4][17]->AddCollider();
    }
    
    {
        mNavWorld = std::make_shared<NavWorld>();
        mNavWorld->InitializeNavWorld(mTileMap);
        mNavWorld->SetStartNode(4, 0);
        mNavWorld->SetEndNode(4, 17);
    }
    //key bindings
    {
        mGame.GetInput().BindAction("Select", IE_Pressed, this, &GameMode::SelectTile);
        mGame.GetInput().BindAction("Num1", IE_Pressed, this, &GameMode::BuildCannonTower);
        mGame.GetInput().BindAction("Num2", IE_Pressed, this, &GameMode::BuildFrostTower);
    }
    
    if(mNavWorld->TryFindPath())
    {
        mNavWorld->MarkPath();
    }
    //initialize HUD
    {
        mMoneyHUD = HUD::Spawn(mGame);
        mMoneyHUD->SetPosition(Vector3(-475.0f, 300.0f, 0.0f));
        
        mHealthHUD = HUD::Spawn(mGame);
        mHealthHUD->SetPosition(Vector3(-475.0f, 250.0f, 0.0f));

        mStatusHUD = HUD::Spawn(mGame);
        mStatusHUD->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        mStatusHUD->GetFontComponent()->SetText("", Vector3(176, 0, 124), 48);
        
        UpdateHUD();
    }
    
    SpawnEnemyWave();
}

void GameMode::UpdateHUD()
{
    mMoneyHUD->GetFontComponent()->SetText("Money: " + NumberToString(mMoney), Vector3(137, 194, 131));
    mHealthHUD->GetFontComponent()->SetText("Health: " + NumberToString(mHealth), Vector3(236, 77, 76));

}

void GameMode::UpdateMessage(std::string message)
{
    mGame.GetGameTimerManager().ClearTimer(messageTimerHandle);
    mStatusHUD->GetFontComponent()->SetText(message, Vector3(255, 223, 28), 52);
    mGame.GetGameTimerManager().SetTimer(messageTimerHandle, this, &GameMode::ClearMessage, 1.0f);
}

void GameMode::ClearMessage()
{
    mStatusHUD->GetFontComponent()->SetText("", Vector3(176, 0, 124));
}

void GameMode::SpawnEnemyWave(){
    
    if (mHealth >0) {
        float singleEnemyInterval = 0.5f;
        
        std::cout << "spawning enenmy: " << waveNum*baseEnemyNum << std::endl;
        for (int i = 0; i < waveNum * baseEnemyNum; i++) {
            TimerHandle th;
            mGame.GetGameTimerManager().SetTimer(th, this, &GameMode::SpawnSingleEnemy, singleEnemyInterval*i);
            if (i == waveNum * baseEnemyNum-1) {
                TimerHandle th;
                mGame.GetGameTimerManager().SetTimer(th, this, &GameMode::SpawnEnemyWave, 5.0f + singleEnemyInterval*i);
            }
        }
        waveNum+=1;
    }
}

void GameMode::SpawnSingleEnemy(){
    auto enemy = Enemy::Spawn(mGame);
    enemy->SetPath(mNavWorld->startNode);
    
}

void GameMode::SelectTile()
{
    bool selected = false;
    
    Vector2 mousePos = mGame.GetInput().GetMousePosition();
    mousePos -= Vector2(512.0f, 384.0f);
    mousePos.Set(mousePos.x, -mousePos.y);
    
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 18; ++j) {
            Vector3 tilePose = mTileMap[i][j]->GetPosition();
            if (mTileMap[i][i]->isSpecial) {
                continue;
            }
            if ( (tilePose.x-25 < mousePos.x && mousePos.x < tilePose.x + 25)
                && (tilePose.y-25 < mousePos.y && mousePos.y < tilePose.y + 25)
                )
            {
                if (mChosenTile != nullptr) {
                    mChosenTile->SwitchTile(0);
                }
                mTileMap[i][j]->SwitchTile(1);
                mChosenTile = mTileMap[i][j];
                selected = true;
            }
        }
    }
    
    if (mHealth <= 0) {
        selected = false;
    }
    
    if (!selected) {
        if (mChosenTile != nullptr) {
            mChosenTile->SwitchTile(0);
        }
        mChosenTile = nullptr;
    }
}

void GameMode::BuildCannonTower()
{
    if (mChosenTile != nullptr)
    {
        mChosenTile->isBlocked = true;
        
        if(!mNavWorld->TryFindPath())
        {
            mChosenTile->isBlocked = false;
            UpdateMessage("Cannont Block The Entire Path");
        }
        if (mMoney < cannonTowerCost)
        {
            mChosenTile->isBlocked = false;
            UpdateMessage("Need More Money");
        }
        else
        {
            mMoney -= cannonTowerCost;
            UpdateHUD();
            
            mNavWorld->MarkPath();
            mChosenTile->SpawnCannonTower();
        }
        
    }
}

void GameMode::BuildFrostTower()
{
    if (mChosenTile != nullptr)
    {
        mChosenTile->isBlocked = true;
        
        if(!mNavWorld->TryFindPath())
        {
            mChosenTile->isBlocked = false;
            UpdateMessage("Cannont Block The Entire Path");
        }
        if (mMoney < frostTowerCost)
        {
            mChosenTile->isBlocked = false;
            UpdateMessage("Need More Money");
        }
        else
        {
            mMoney -= frostTowerCost;
            UpdateHUD();
            
            mNavWorld->MarkPath();
            mChosenTile->SpawnFrostTower();
        }
        
    }
}



