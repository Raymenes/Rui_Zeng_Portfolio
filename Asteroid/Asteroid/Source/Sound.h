//
//  Sound.h
//  Game-mac
//
//  Created by Rui Zeng on 9/16/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Asset.h"

class Sound : public Asset
{
    DECL_ASSET(Sound, Asset);
    
public:
    Sound();
    
    ~Sound();
    
    bool Load(const char* fileName, class AssetCache* cache) override;
    
    struct Mix_Chunk* GetData(){return mData;}
    
private:
    struct Mix_Chunk* mData;
};

DECL_PTR(Sound);
