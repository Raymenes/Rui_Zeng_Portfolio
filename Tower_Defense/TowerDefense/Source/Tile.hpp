//
//  Tile.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/2/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "Actor.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SphereCollision.h"
#include "Tower.hpp"
#include "CannonTower.hpp"
#include "FrostTower.hpp"

class Tile : public Actor
{
    DECL_ACTOR(Tile, Actor);
public:
    Tile(Game& game);
    
    void SwitchTile(int i){ mMeshComponent->SetMeshIndex(i); }
    
    int test;
    
    void AddCollider(){
        auto col = SphereCollision::Create(*this);
        col->RadiusFromMesh(mMesh);
        col->SetScale(0.01f);
         }
    
    void SpawnCannonTower(){
        if(mTower == nullptr){
            auto tower = CannonTower::Spawn(mGame);
            tower->SetPosition(GetPosition());
            tower->SetCannonPosition(GetPosition());
            mTower = tower;
        }
    }
    
    void SpawnFrostTower(){
        if(mTower == nullptr){
            mTower = FrostTower::Spawn(mGame);
            mTower->SetPosition(GetPosition());
        }
    }
    
    bool IsBlocked(){ return isBlocked; }
    
    bool isBlocked;
    
    bool isSpecial;
    
    bool isPath;
    
private:
    MeshComponentPtr mMeshComponent;
    MeshPtr mMesh;
    TowerPtr mTower;
    
};

DECL_PTR(Tile);