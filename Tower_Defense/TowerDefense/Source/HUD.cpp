//
//  HUD.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/5/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "HUD.hpp"
#include "Game.h"


IMPL_ACTOR(HUD, Actor);

HUD::HUD(Game& game)
    : Actor(game)
{
    mFontComponent = FontComponent::Create(*this);
    mFont = mGame.GetAssetCache().Load<Font>("Fonts/Carlito-Regular.ttf");
    mFontComponent->SetFont(mFont);
}