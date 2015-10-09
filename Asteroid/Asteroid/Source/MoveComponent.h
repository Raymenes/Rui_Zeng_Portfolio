// MoveComponent.h
// Component that does basic velocity-based movement
// Generally used as a "pre-tick" component

#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
	DECL_COMPONENT(MoveComponent, Component);
public:
	MoveComponent(Actor& owner);

	void Tick(float deltaTime) override;

	// Linear speed (in game units/s)
	void SetLinearSpeed(float speed) { mLinearSpeed = speed; }
	float GetLinearSpeed() const { return mLinearSpeed; }

	// Angular speed (in radians/s)
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	float GetAngularSpeed() const { return mAngularSpeed; }

	// AddTo calls should always be +1.0f or -1.0f
	void AddToLinearAxis(float delta);
	void AddToAngularAxis(float delta);

	void SetLinearAxis(float axis);
	float GetLinearAxis() const { return mLinearAxis; }

	void SetAngularAxis(float axis);
	float GetAngularAxis() const { return mAngularAxis; }
private:
	Vector3 mVelocity;

	float mLinearSpeed;
	float mAngularSpeed;

	float mLinearAxis;
	float mAngularAxis;
};

DECL_PTR(MoveComponent);
