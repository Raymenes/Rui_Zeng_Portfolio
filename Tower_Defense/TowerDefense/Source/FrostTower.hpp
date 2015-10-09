//
//  FrostTower.hpp
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

class FrostTower : public Tower
{
    DECL_ACTOR(FrostTower, Tower);
public:
    FrostTower(Game& game);
    
    void AttackEnemy();
private:
    MeshComponentPtr mMeshComponent;
};
DECL_PTR(FrostTower);


