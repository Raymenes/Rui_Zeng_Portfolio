//
//  Laser.h
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

class Laser : public Actor
{
    DECL_ACTOR(Laser, Actor);
public:
    Laser(Game& game);
    
    MoveComponentPtr GetMoveComponent(){return mMoveComponent;}
    
    void Tick(float deltaTime) override;
    
    void BeginTouch(Actor& other) override;
    
    void BeginPlay() override;
    
    void OnDieTimer(){ SetIsAlive(false); }

private:
    MoveComponentPtr mMoveComponent;
    AudioComponentPtr mAudioComponent;
    SoundPtr mCollisionSnd;
};

DECL_PTR(Laser);