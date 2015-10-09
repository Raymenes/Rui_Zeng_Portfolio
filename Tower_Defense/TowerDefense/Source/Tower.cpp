//
//  Tower.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/3/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "Tower.hpp"
#include "Game.h"

IMPL_ACTOR(Tower, Actor);

Tower::Tower(Game& game)
    :Actor(game)
{
    
    mMeshComponent = MeshComponent::Create(*this);
    auto mesh = game.GetAssetCache().Load<Mesh>("Meshes/TowerBase.itpmesh2");
    mMeshComponent->SetMesh(mesh);
    
}
