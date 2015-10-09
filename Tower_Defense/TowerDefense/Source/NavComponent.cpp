//
//  NavComponent.cpp
//  Game-mac
//
//  Created by Rui Zeng on 10/5/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#include "NavComponent.hpp"
#include "Game.h"
#include "Actor.h"

IMPL_COMPONENT(NavComponent, MoveComponent);

NavComponent::NavComponent(Actor& owner)
: MoveComponent(owner)
{
    mNextNode = nullptr;
}

void NavComponent::Tick(float deltaTime)
{
    //handling liner movement
    if(!Math::IsZero(GetLinearAxis()) && mNextNode!=nullptr)
    {
        const Vector3 ownerPos = mOwner.GetPosition();
        
        Vector3 mPos = mOwner.GetPosition();
        Vector3 destination = mNextNode->mTile->GetPosition();
        float distance2 = (destination - mPos).LengthSq();
        
        Vector3 direction = mOwner.GetForward();
        Vector3 velocity = direction * GetLinearAxis() * GetLinearSpeed();
        Vector3 nextFramePos = ownerPos + velocity * deltaTime;
        
        float nextFrameDis2 = (nextFramePos - mPos).LengthSq();
        
        if (nextFrameDis2 > distance2) {
            
            mOwner.SetPosition(destination);
            mNextNode = mNextNode->mParent;
            if (mNextNode != nullptr) {
                float angle = mOwner.ComputAngleToTarget(*mNextNode->mTile);
                mOwner.SetRotation(angle);
            }
            
        }else{
            mOwner.SetPosition(nextFramePos);
        }
    }
    
    //handling angular movement
    if (!Math::IsZero(GetAngularAxis())) {
        auto mAngularVelocity = GetAngularAxis() * GetAngularSpeed();
        mOwner.SetRotation(mOwner.GetRotation()+mAngularVelocity * deltaTime);
    }

}

void NavComponent::FollowPath(PathNodePtr startNode)
{
    
    mOwner.SetPosition(startNode->mTile->GetPosition());
    mNextNode = startNode->mParent;
    if (mNextNode != nullptr) {
        float angle = mOwner.ComputAngleToTarget(*mNextNode->mTile);
        mOwner.SetRotation(angle);
    }
}