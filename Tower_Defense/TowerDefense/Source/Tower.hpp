//
//  Tower.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Actor.h"
#include "MeshComponent.h"

class Tower : public Actor
{
    DECL_ACTOR(Tower, Actor);
public:
    Tower(Game& game);
private:
    MeshComponentPtr mMeshComponent;
};
DECL_PTR(Tower);

