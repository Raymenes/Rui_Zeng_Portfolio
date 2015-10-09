//
//  Asteroid.h
//  Game-mac
//
//  Created by Rui Zeng on 9/6/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include "Actor.h"
#include <stdio.h>

class Asteroid : public Actor
{
    DECL_ACTOR(Asteroid, Actor);
    
public:
    Asteroid(Game& game);
};

DECL_PTR(Asteroid);
