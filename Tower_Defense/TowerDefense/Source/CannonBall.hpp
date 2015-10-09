//
//  CannonBall.hpp
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

class CannonBall : public Actor
{
    DECL_ACTOR(CannonBall, Actor);
public:
    CannonBall(Game& game);
    
    void Die(){SetIsAlive(false);}
    
private:
    MoveComponentPtr mMoveComponent;
    SphereCollisionPtr mSphereCollider;
    MeshComponentPtr mMeshComponent;
};

DECL_PTR(CannonBall)