#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

IMPL_COMPONENT(MoveComponent, Component);

MoveComponent::MoveComponent(Actor& owner)
	:Component(owner)
	,mLinearSpeed(0.0f)
	,mAngularSpeed(0.0f)
	,mLinearAxis(0.0f)
	,mAngularAxis(0.0f)
{

}

void MoveComponent::Tick(float deltaTime)
{
    //handling liner movement
    if(!Math::IsZero(mLinearAxis)){
        const Vector3 ownerPos = mOwner.GetPosition();
        
        mVelocity = mOwner.GetForward() * mLinearAxis * mLinearSpeed;
        mOwner.SetPosition(ownerPos + mVelocity * deltaTime);
        
//        if (ownerPos.x > 512) {
//            mOwner.SetPosition(Vector3(-ownerPos.x+2, ownerPos.y, ownerPos.z));
//        }
//        else if (ownerPos.x  < -512) {
//             mOwner.SetPosition(Vector3(-ownerPos.x-2, ownerPos.y, ownerPos.z));
//        }
//        if (ownerPos.y > 384)
//        {
//            mOwner.SetPosition(Vector3(ownerPos.x, -ownerPos.y+2, ownerPos.z));
//        }
//        else if (ownerPos.y  < -384) {
//            mOwner.SetPosition(Vector3(ownerPos.x, -ownerPos.y-2, ownerPos.z));
//
//        }
    }
    
    //handling angular movement
    if (!Math::IsZero(mAngularAxis)) {
        auto mAngularVelocity = mAngularAxis * mAngularSpeed;
        mOwner.SetRotation(mOwner.GetRotation()+mAngularVelocity * deltaTime);
    }
}

void MoveComponent::AddToLinearAxis(float delta)
{
	mLinearAxis += delta;
	mLinearAxis = Math::Clamp(mLinearAxis, -1.0f, 1.0f);
}

void MoveComponent::AddToAngularAxis(float delta)
{
	mAngularAxis += delta;
	mAngularAxis = Math::Clamp(mAngularAxis, -1.0f, 1.0f);
}

void MoveComponent::SetLinearAxis(float axis)
{
	mLinearAxis = Math::Clamp(axis, -1.0f, 1.0f);
}

void MoveComponent::SetAngularAxis(float axis)
{
	mAngularAxis = Math::Clamp(axis, -1.0f, 1.0f);
}


