//
//  Asteroid.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/6/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "Asteroid.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Random.h"
#include "MoveComponent.h"
#include "SphereCollision.h"

IMPL_ACTOR(Asteroid, Actor);

Asteroid::Asteroid(Game& game) : Actor(game)
{
    auto meshComponent = MeshComponent::Create(*this);
    auto mesh = game.GetAssetCache().Load<Mesh>("Meshes/AsteroidMesh.itpmesh2");
    auto texture = game.GetAssetCache().Load<Texture>("Textures/Asteroid.png");
    meshComponent->SetMesh(mesh);
    
    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));
    
    auto move = MoveComponent::Create(*this, Component::PreTick);
    move -> SetLinearSpeed(150.0f);
    move -> SetLinearAxis(1.0f);
    
    auto col = SphereCollision::Create(*this);
    col->SetScale(0.9f);
    col->RadiusFromTexture(texture);
}


