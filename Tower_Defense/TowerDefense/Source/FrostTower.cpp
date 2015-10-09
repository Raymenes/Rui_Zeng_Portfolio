//
//  FrostTower.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "FrostTower.hpp"
#include "Game.h"
#include <vector>

IMPL_ACTOR(FrostTower, Tower);

FrostTower::FrostTower(Game& game)
:Tower(game)
{
    mMeshComponent = MeshComponent::Create(*this);
    auto mesh = game.GetAssetCache().Load<Mesh>("Meshes/Frost.itpmesh2");
    mMeshComponent->SetMesh(mesh);
    
    TimerHandle th;
    mGame.GetGameTimerManager().SetTimer(th, this, &FrostTower::AttackEnemy, 2.0f, true);
}

void FrostTower::AttackEnemy()
{
    std::vector<Enemy*> target = mGame.GetWorld().GetEnemiesInRange(GetPosition(), 150.0f);
    for(auto e : target){
        e->Slow();
    }
}