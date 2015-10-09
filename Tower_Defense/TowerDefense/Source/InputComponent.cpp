//
//  InputComponent.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/16/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "InputComponent.h"
#include "Actor.h"
#include "Game.h"

IMPL_COMPONENT(InputComponent, MoveComponent);


void InputComponent::BindLinearAxis(const std::string& name)
{
    mOwner.GetGame().GetInput().BindAxis(name, this, &InputComponent::OnLinearAxis);
}
void InputComponent::BindAngularAxis(const std::string& name)
{
    mOwner.GetGame().GetInput().BindAxis(name, this, &InputComponent::OnAngularAxis);
}
void InputComponent::OnLinearAxis(float value)
{
    SetLinearAxis(value);
}
void InputComponent::OnAngularAxis(float value)
{
    SetAngularAxis(value);
}