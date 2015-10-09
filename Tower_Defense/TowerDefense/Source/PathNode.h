//
//  PathNode.h
//  Game-mac
//
//  Created by Rui Zeng on 10/4/15.
//  Copyright © 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Tile.hpp"
#include "Math.h"
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <iostream>

struct PathNode
{
    std::vector< std::shared_ptr<PathNode> > adjacentNodes;
    TilePtr mTile;
    Vector2 mGridPos;
    std::shared_ptr<PathNode> mParent;
    int fx, gx, hx;
    bool isBlocked;
    
    PathNode()
    :mParent(nullptr), isBlocked(false), fx(INT32_MAX), gx(INT32_MAX), hx(INT32_MAX)
    {}
    
    bool operator==(PathNode rhs)
    {
        return ( (mGridPos.x == rhs.mGridPos.x) && (mGridPos.y == rhs.mGridPos.y) );
    }
};
DECL_PTR(PathNode);

class NavWorld
{
public:
    void InitializeNavWorld(TilePtr tileMap[][18]){
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 18; ++j) {
                mNavMap[i][j] = std::make_shared<PathNode>() ;
                mNavMap[i][j]->mTile = tileMap[i][j];
                mNavMap[i][j]->mGridPos = Vector2(i, j);
            }
        }
        ConnectNodes();
    }
    
    void ConnectNodes(){
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 18; ++j) {
                if (i > 0) {
                    mNavMap[i][j]->adjacentNodes.push_back(mNavMap[i-1][j]);
                }
                if (i < 8) {
                    mNavMap[i][j]->adjacentNodes.push_back(mNavMap[i+1][j]);
                }
                if (j > 0) {
                    mNavMap[i][j]->adjacentNodes.push_back(mNavMap[i][j-1]);
                }
                if (j < 17) {
                    mNavMap[i][j]->adjacentNodes.push_back(mNavMap[i][j+1]);
                }
            }
        }
    }
    
    bool TryFindPath()
    {
        bool ret = true;
        PathNodePtr currentNode = endNode;
        std:: unordered_set<PathNodePtr> closedSet;
        std:: unordered_set<PathNodePtr> openSet;
        
        closedSet.emplace(currentNode);
        while (! ( (*currentNode)==(*startNode) ) ) {
            auto q = currentNode->adjacentNodes.begin();
            while(q != currentNode->adjacentNodes.end())
            {
                
                auto p = *q;
                if(!p->mTile->IsBlocked())
                {
                    if(closedSet.find(p) != closedSet.end())
                    {
                        q++;
                        continue;
                    }
                    else if (openSet.find(p) != openSet.end())
                    {
                        int new_gx = currentNode->gx + 1;
                        if (new_gx < p->gx) {
                            p->gx = new_gx;
                            p->fx = p->hx + p->gx;
                        }
                    }
                    else
                    {
                        p->mParent = currentNode;
                        p->hx = ComputerH(p, endNode);
                        p->gx = currentNode->gx + 1;
                        p->fx = p->hx + p->gx;
                        openSet.emplace(p);
                    }
                }
                q ++;
            }
            if( openSet.empty() )
            { ret = false; break; }
            
            auto p = openSet.begin();
            currentNode = *p;
            while(p != openSet.end())
            {
                if ( (*p)->fx < currentNode->fx ) {
                    currentNode = (*p);
                }
                p++;
            }
            openSet.erase(currentNode);
            closedSet.emplace(currentNode);
        }
        return ret;
    }
    
    int ComputerH(PathNodePtr curr, PathNodePtr target){
        int x = curr->mGridPos.x - target->mGridPos.x;
        if(x < 0)
            x = -1 * x;
        int y = curr->mGridPos.y - target->mGridPos.y;
        if (y < 0) {
            y = -1 * y;
        }
        return (x + y);
    }
    
    void MarkPath()
    {
        //if it's previous path, unmark it
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 18; ++j) {
                if (mNavMap[i][j]->mTile->isPath)
                {
                    mNavMap[i][j]->mTile->SwitchTile(0);
                    mNavMap[i][j]->mTile->isPath = false;
                }
            }
        }

        
        PathNodePtr currNode = startNode;
        while( !((*currNode) == (*endNode)) && currNode->mParent!= nullptr )
        {
            if(!currNode->mTile->isSpecial)
            {
                currNode->mTile->isPath = true;
                currNode->mTile->SwitchTile(4);

            }
            currNode = currNode->mParent;
        }
    }
    
    void SetStartNode(int i, int j){ startNode = mNavMap[i][j];}
    void SetEndNode(int i, int j){ endNode = mNavMap[i][j];}

    PathNodePtr mNavMap[9][18];
    PathNodePtr startNode;
    PathNodePtr endNode;
};
DECL_PTR(NavWorld);