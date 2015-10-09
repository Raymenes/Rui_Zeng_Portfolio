//
//  AudioComponent.cpp
//  Game-mac
//
//  Created by Rui Zeng on 9/16/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "AudioComponent.h"
#include "Actor.h"
#include <SDL/SDL_mixer.h>

IMPL_COMPONENT(AudioComponent, Component);

AudioComponent::AudioComponent(Actor& owner)
    : Component(owner)
{
    
}

SoundCue AudioComponent::PlaySound(SoundPtr sound, bool looping)
{
    int loopingID;
    
    if (looping){loopingID = -1;}
    else        {loopingID = 0;}
    
    int currPlaying = Mix_PlayChannel(-1, sound->GetData(), loopingID);
    
    return SoundCue(currPlaying);
}

SoundCue::SoundCue(){
    mChannel = -1;
}

SoundCue::SoundCue(int channel){
    mChannel = channel;
}

void SoundCue::Pause(){
    if (mChannel == -1) {
        return;
    }
    Mix_Pause(mChannel);
}

void SoundCue::Resume(){
    if (mChannel == -1) {
        return;
    }
    Mix_Resume(mChannel);
}

void SoundCue::Stop(){
    if (mChannel == -1) {
        return;
    }
    Mix_HaltChannel(mChannel);
}

bool SoundCue::isPlaying(){
    if (mChannel == -1) {
        return false;
    }
    return Mix_Playing(mChannel) == 1;
}

bool SoundCue::isPaused(){
    if (mChannel == -1) {
        return false;
    }
    return Mix_Paused(mChannel) == 1;
}