//
//  HUD.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/5/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Actor.h"
#include "FontComponent.h"
#include "Font.h"
#include <stdio.h>

class HUD : public Actor
{
    DECL_ACTOR(HUD, Actor);
public:
    HUD(Game& game);
    
    FontComponentPtr& GetFontComponent(){return mFontComponent;};
    
private:
    FontComponentPtr mFontComponent;
    FontPtr mFont;
};

DECL_PTR(HUD);