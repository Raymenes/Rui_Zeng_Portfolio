//
//  Ship.h
//  Game-mac
//
//  Created by Rui Zeng on 9/7/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Actor.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "Sound.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "InputComponent.h"

class Ship : public Actor
{
    DECL_ACTOR(Ship, Actor);
    
public:
    Ship(Game& game);
    
    InputComponentPtr GetMoveComponent(){return mMoveComponent;}
    
    void FireMissile();
    
    void Tick(float deltaTime) override;
    
    void BeginPlay() override;
    
    void BeginTouch(Actor& other) override;
    
    void OnSpawnShip();
    
    void OnInvulnerableOff();
    
private:
    InputComponentPtr mMoveComponent;
    AudioComponentPtr mAudioComponent;
    MeshComponentPtr mMeshComponent;
    
    SoundPtr mLaserSnd;
    SoundPtr mEngineSnd;
    SoundPtr mDeathSnd;
    SoundCue mEngineSndCue;
    TexturePtr staticTexture;
    TexturePtr movingTexture;
    MeshPtr mShipMesh;
    
    bool isInvulnerable;
};

DECL_PTR(Ship);