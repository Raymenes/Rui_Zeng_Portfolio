//
//  Laser.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/7/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "Laser.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "SphereCollision.h"
#include "CollisionComponent.h"
#include "Asteroid.h"
#include "AudioComponent.h"
#include "Sound.h"
#include "GameTimers.h"

IMPL_ACTOR(Laser, Actor);

Laser::Laser(Game& game) : Actor(game)
{
    auto sprite = SpriteComponent::Create(*this);
    auto texture = game.GetAssetCache().Load<Texture>("Textures/Laser.png");
    sprite->SetTexture(texture);
    
    SetRotation(Math::PiOver2);
    
    mMoveComponent = MoveComponent::Create(*this, Component::PreTick);
    mMoveComponent -> SetLinearSpeed(600.0f);
    mMoveComponent -> SetLinearAxis(1.0f);
    
    mAudioComponent = AudioComponent::Create(*this);
    mCollisionSnd = game.GetAssetCache().Load<Sound>("Sounds/AsteroidDie.wav");
    
    auto col = SphereCollision::Create(*this);
    col->SetScale(0.9f);
    col->RadiusFromTexture(texture);
    
}

void Laser::Tick(float deltaTime)
{
    
}

void Laser::BeginPlay()
{
    TimerHandle deathHandle;
    mGame.GetGameTimerManager().SetTimer(deathHandle, this, &Laser::OnDieTimer, 1.0f);
}

void Laser:: BeginTouch(Actor& other)
{
    if (IsAlive() && IsA<Asteroid>(other)) {
        SetIsAlive(false);
        other.SetIsAlive(false);
        if(other.GetWorldTransform().GetScale().x >= 0.7){
            for (int i = 0; i < 4; ++i) {
                auto ast = Asteroid::Spawn(mGame);
                ast->SetPosition(GetWorldTransform().GetTranslation());
                ast->SetScale(0.25f);
            }
        }
        mAudioComponent->PlaySound(mCollisionSnd);
    }
}
