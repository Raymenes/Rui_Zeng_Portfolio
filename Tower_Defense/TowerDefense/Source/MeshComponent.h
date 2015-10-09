//
//  MeshComponent.h
//  Game-mac
//
//  Created by Rui Zeng on 9/18/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "DrawComponent.h"
#include "Mesh.h"

class MeshComponent : public DrawComponent
{
    DECL_COMPONENT(MeshComponent, DrawComponent);
public:
    
    MeshComponent(Actor& owner);
    
    void Draw(class Renderer& render) override;
        
    void SetMesh( MeshPtr mesh ){ mMesh = mesh; }
    
    MeshPtr GetMesh() { return mMesh; }
    
    void SetMeshIndex(int index){ mMeshIndex = index; }
    
private:
    MeshPtr mMesh;
    int mMeshIndex;
};

DECL_PTR(MeshComponent);