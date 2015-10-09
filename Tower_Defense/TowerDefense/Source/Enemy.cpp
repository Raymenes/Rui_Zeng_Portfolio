//
//  Enemy.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "Enemy.hpp"
#include "Game.h"
#include "CannonBall.hpp"

IMPL_ACTOR(Enemy, Actor);

Enemy::Enemy(Game& game)
: Actor(game), mHealth(100)
{
    mNavComponent = NavComponent::Create(*this);
    mNavComponent->SetLinearSpeed(40.0f);
    mNavComponent->SetLinearAxis(1.0f);
    
    mMeshComponent = MeshComponent::Create(*this);
    auto mesh = game.GetAssetCache().Load<Mesh>("Meshes/Peasant.itpmesh2");
    mMeshComponent->SetMesh(mesh);
    
    mSphereCollider = SphereCollision::Create(*this);
    mSphereCollider->RadiusFromMesh(mesh);
    mSphereCollider->SetScale(0.01f);
    
}


void Enemy::BeginTouch(Actor& other)
{
    if (IsA<Tile>(other)) {
        mGame.GetGameMode()->LoseHealth();
        SetIsAlive(false);

    }
    if (IsA<CannonBall>(other)) {
        ReceiveAttack();
        other.SetIsAlive(false);
    }
}

void Enemy::BeginPlay(){mGame.GetWorld().AddEnemy(this);}

void Enemy::EndPlay(){mGame.GetWorld().RemoveEnemy(this);}

void Enemy::Slow(){
    if (mNavComponent->GetLinearAxis() > 0.5f) {
        mNavComponent->SetLinearAxis(0.5f);
        TimerHandle th;
        mGame.GetGameTimerManager().SetTimer(th, this, &Enemy::UnSlow, 1.0f);
        SwitchTexture(1);
    }
}

void Enemy::UnSlow(){
    if (IsAlive()) {
        mNavComponent->SetLinearAxis(1.0f);
        SwitchTexture(0);
    }
}

void Enemy::ReceiveAttack(){
    mHealth -= 55;
    if (mHealth <= 0) {
        mGame.GetGameMode()->GetMoney();
        SetIsAlive(false);
        mGame.GetGameTimerManager().ClearAllTimers(this);
    }
}


void Enemy::Tick(float deltaTime)
{
    
}