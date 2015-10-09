#include "SphereCollision.h"
#include "Actor.h"


IMPL_COMPONENT(SphereCollision, CollisionComponent);

SphereCollision::SphereCollision(Actor& owner)
	:CollisionComponent(owner)
	,mOriginalRadius(1.0f)
	,mActualRadius(1.0f)
	,mScale(1.0f)
{

}

void SphereCollision::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	// Update the radius/center from our owning component
    mCenter = mOwner.GetWorldTransform().GetTranslation();
    mActualRadius = mOwner.GetWorldTransform().GetScale().x * mOriginalRadius;
    
	// Scale the radius by the world transform scale
	// (We can assume the scale is uniform, because
	// actors don't allow non-uniform scaling)
	// TODO
}

bool SphereCollision::Intersects(CollisionComponentPtr other)
{
    if (IsA<SphereCollision>(other)) {
        return IntersectsSphere(Cast<SphereCollision>(other));
    }
	return false;
}

void SphereCollision::RadiusFromTexture(TexturePtr texture)
{
	mOriginalRadius = static_cast<float>(Math::Max(texture->GetHeight() / 2, 
		texture->GetHeight() / 2));
}

void SphereCollision::RadiusFromMesh(MeshPtr mesh)
{
    mOriginalRadius = mesh->GetRadius();
}

bool SphereCollision::IntersectsSphere(SphereCollisionPtr other)
{
    Vector3 difference = other->GetOwner().GetWorldTransform().GetTranslation() - mCenter;
    float distance2 = difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;
    float colDistance2 = powf((mActualRadius+other->GetActualRadius()), 2);
    if (distance2 > colDistance2) {
        return false;
    }
    else
        return true;
}
