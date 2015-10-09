//
//  CannonBall.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "CannonBall.hpp"
#include "Game.h"

IMPL_ACTOR(CannonBall, Actor);

CannonBall::CannonBall(Game& game)
: Actor(game)
{
    mMoveComponent = MoveComponent::Create(*this);
    mMoveComponent->SetLinearSpeed(300.0f);
    mMoveComponent->SetLinearAxis(1.0f);
    
    mMeshComponent = MeshComponent::Create(*this);
    auto mesh = game.GetAssetCache().Load<Mesh>("Meshes/Cannonball.itpmesh2");
    mMeshComponent->SetMesh(mesh);
    
    mSphereCollider = SphereCollision::Create(*this);
    mSphereCollider->RadiusFromMesh(mesh);
    mSphereCollider->SetScale(0.75f);
    
    TimerHandle th;
    game.GetGameTimerManager().SetTimer(th, this, &CannonBall::Die, 3.0f);
}

