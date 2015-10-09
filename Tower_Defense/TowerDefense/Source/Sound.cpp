//
//  Sound.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/16/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "Sound.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>


Sound::Sound(){
    mData = nullptr;
}

Sound::~Sound(){
    if (mData == nullptr) {
        return;
    }
    Mix_FreeChunk(mData);
}

bool Sound::Load(const char *fileName, class AssetCache *cache){
    mData = Mix_LoadWAV(fileName);
    if (mData == nullptr) {
        SDL_Log("Have problem loading ");
        SDL_Log(fileName);
        return false;
    }
    return true;
}
