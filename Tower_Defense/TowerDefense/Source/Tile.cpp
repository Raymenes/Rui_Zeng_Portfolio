//
//  Tile.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/2/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "Tile.hpp"
#include "Game.h"

IMPL_ACTOR(Tile, Actor);

Tile::Tile(Game& game)
    : Actor(game), isBlocked(false), isPath(false), isSpecial(false)
{
    mMeshComponent = MeshComponent::Create(*this);
    mMesh = game.GetAssetCache().Load<Mesh>("Meshes/Tile.itpmesh2");
    mMeshComponent->SetMesh(mMesh);
}