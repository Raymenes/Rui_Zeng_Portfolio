//
//  NavComponent.hpp
//  Game-mac
//
//  Created by Rui Zeng on 10/5/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "MoveComponent.h"
#include "PathNode.h"

class NavComponent : public MoveComponent
{
    DECL_COMPONENT(NavComponent, MoveComponent);
public:
    NavComponent(Actor& owner);
    
    void Tick(float deltaTime) override;
    
    void FollowPath(PathNodePtr startNode);
    
private:
    PathNodePtr mNextNode;
};

DECL_PTR(NavComponent);