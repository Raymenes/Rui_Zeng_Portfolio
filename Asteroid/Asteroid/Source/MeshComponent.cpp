//
//  MeshComponent.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/18/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "MeshComponent.h"
#include "Actor.h"
#include <SDL/SDL.h>
#include "Renderer.h"

IMPL_COMPONENT(MeshComponent, DrawComponent);

MeshComponent::MeshComponent(Actor& owner)
: DrawComponent(owner)
{}

void MeshComponent::Draw(Renderer &render)
{
    if (mMesh != nullptr) {
        render.DrawBasicMesh(mMesh->GetVertexArray(), mMesh->GetTexture(0), mOwner.GetWorldTransform());
    }
}