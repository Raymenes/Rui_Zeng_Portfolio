//
//  CannonTower.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "CannonTower.hpp"
#include "Game.h"
#include "Math.h"
#include "CannonBall.hpp"

IMPL_ACTOR(CannonTower, Tower);

CannonTower::CannonTower(Game& game)
:Tower(game)
{
    auto mesh = game.GetAssetCache().Load<Mesh>("Meshes/Cannon.itpmesh2");

    mCannon = Actor::Spawn(mGame);
    mMeshComponent = MeshComponent::Create(*mCannon);
    mCannon->AddComponent(mMeshComponent);
    mMeshComponent->SetMesh(mesh);
    
    TimerHandle th;
    mGame.GetGameTimerManager().SetTimer(th, this, &CannonTower::AttackEnemy, 2.0f, true);
}

void CannonTower::AttackEnemy()
{
    Enemy* target = mGame.GetWorld().GetClosestEnemy(GetPosition(), 150.0f);
    if (target != nullptr) {

        float angle = ComputAngleToTarget(*target);
        mCannon->SetRotation(angle);
        auto ball = CannonBall::Spawn(mGame);
        ball->SetPosition(GetPosition());
        ball->SetRotation(angle);
    }
}

