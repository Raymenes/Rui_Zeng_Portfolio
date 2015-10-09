//
//  Ship.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/7/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "Ship.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "SphereCollision.h"
#include "Asteroid.h"
#include "Laser.h"
#include "Sound.h"
#include "GameTimers.h"
#include <iostream>

IMPL_ACTOR(Ship, Actor);

Ship::Ship(Game& game) : Actor(game)
    ,isInvulnerable(false)
{
    mMeshComponent = MeshComponent::Create(*this);
    mShipMesh = game.GetAssetCache().Load<Mesh>("Meshes/PlayerShip.itpmesh2");
    staticTexture = game.GetAssetCache().Load<Texture>("Textures/Spaceship.png");
    movingTexture = game.GetAssetCache().Load<Texture>("Textures/SpaceshipWithThrust.png");
    
    mMeshComponent->SetMesh(mShipMesh);
    
    SetScale(0.5f);
    
    SetRotation(Math::PiOver2);
    
    mMoveComponent = InputComponent::Create(*this, Component::PreTick);
    mMoveComponent -> SetLinearSpeed(400.0f);
    mMoveComponent -> SetLinearAxis(0.0f);
    mMoveComponent -> SetAngularSpeed(Math::TwoPi);
    
    auto col = SphereCollision::Create(*this);
    col->SetScale(0.75f);
    col->RadiusFromMesh(mShipMesh);
    
    mEngineSnd = game.GetAssetCache().Load<Sound>("Sounds/ShipEngine.wav");
    mLaserSnd = game.GetAssetCache().Load<Sound>("Sounds/Laser.wav");
    mDeathSnd = game.GetAssetCache().Load<Sound>("Sounds/ShipDie.wav");
    mAudioComponent = AudioComponent::Create(*this);
    mEngineSndCue = mAudioComponent->PlaySound(mEngineSnd, true);
    mEngineSndCue.Pause();
}

void Ship:: FireMissile(){
    if (!IsPaused()) {
        auto lsr = Laser::Spawn(mGame);
        lsr->SetRotation(GetRotation());
        lsr->SetPosition(GetPosition());
        mAudioComponent->PlaySound(mLaserSnd);
    }
}

void Ship::Tick(float deltaTime){
    Super::Tick(deltaTime);
    if (IsPaused()) {
        mEngineSndCue.Pause();
    }
    else if (Math::IsZero(mMoveComponent->GetLinearAxis()) ) {
        mEngineSndCue.Pause();
    }
    else{
        mEngineSndCue.Resume();
    }
}

void Ship::BeginPlay(){
    mGame.GetInput().BindAction("Fire", IE_Pressed, this, &Ship::FireMissile);
    mMoveComponent->BindLinearAxis("Move");
    mMoveComponent->BindAngularAxis("Rotate");
}

void Ship::BeginTouch(Actor &other)
{
    if (IsPaused() || isInvulnerable) {
        return;
    }
    else
    {
        if (IsA<Asteroid>(other))
        {
            mAudioComponent->PlaySound(mDeathSnd);
            SetIsPaused(true);
            mMeshComponent->SetIsVisible(false);
            TimerHandle tHandle;
            mGame.GetGameTimerManager().SetTimer(tHandle, this, &Ship::OnSpawnShip, 1.0f);
        }
    }
    
}

void Ship::OnSpawnShip()
{
    SetIsPaused(false);
    mMeshComponent->SetIsVisible(true);
    SetRotation(Math::PiOver2);
    SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    isInvulnerable = true;
    TimerHandle tHandle;
    mGame.GetGameTimerManager().SetTimer(tHandle, this, &Ship::OnInvulnerableOff, 1.5f);

}

void Ship::OnInvulnerableOff()
{
    isInvulnerable = false;
}

