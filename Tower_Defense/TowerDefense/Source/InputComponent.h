//
//  InputComponent.h
//  Game-mac
//
//  Created by Rui Zeng on 9/16/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "Component.h"
#include "MoveComponent.h"

class InputComponent : public MoveComponent
{
    DECL_COMPONENT(InputComponent, MoveComponent);
    
public:
    InputComponent(Actor& owner) : MoveComponent(owner){}
    
    void BindLinearAxis(const std::string& name);
    void BindAngularAxis(const std::string& name);
    void OnLinearAxis(float value);
    void OnAngularAxis(float value);
    
};

DECL_PTR(InputComponent)