//
//  Enemy.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Actor.h"
#include "MeshComponent.h"
#include "SphereCollision.h"
#include "MoveComponent.h"
#include "NavComponent.hpp"

class Enemy : public Actor
{
    DECL_ACTOR(Enemy, Actor);
public:
    Enemy(Game& game);
    
    void BeginTouch(Actor& other) override;
    
    void BeginPlay() override;

    void EndPlay() override;
    
    void Slow();
    
    void UnSlow();
    
    void SwitchTexture(int i){ mMeshComponent->SetMeshIndex(i); }
    
    void ReceiveAttack();
    
    void Tick(float deltaTime) override;
    
    void SetPath(PathNodePtr startNode){ mNavComponent->FollowPath(startNode); }
    
private:
    NavComponentPtr mNavComponent;
    SphereCollisionPtr mSphereCollider;
    MeshComponentPtr mMeshComponent;
    int mHealth;
};
DECL_PTR(Enemy);