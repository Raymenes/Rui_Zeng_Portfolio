//
//  CannonTower.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Actor.h"
#include "MeshComponent.h"
#include "Tower.hpp"

class CannonTower : public Tower
{
    DECL_ACTOR(CannonTower, Tower);
public:
    CannonTower(Game& game);
    void AttackEnemy();
    void SetCannonPosition(const Vector3& position){mCannon->SetPosition(position);}
private:
    MeshComponentPtr mMeshComponent;
    ActorPtr mCannon;
    
};
DECL_PTR(CannonTower);


