#include "SpriteComponent.h"
#include "Actor.h"
#include <SDL/SDL.h>
#include "Renderer.h"

IMPL_COMPONENT(SpriteComponent, DrawComponent);

SpriteComponent::SpriteComponent(Actor& owner)
	:DrawComponent(owner)
{

}

void SpriteComponent::Draw(Renderer& render)
{
    if(mTexture != nullptr){
        Matrix4 textureTransformMatrix = Matrix4::CreateScale(mTexture->GetWidth(), mTexture->GetHeight(), 1.0f);
        render.DrawSprite(mTexture, textureTransformMatrix * mOwner.GetWorldTransform());
    }
}

