//
//  AudioComponent.h
//  Game-mac
//
//  Created by Rui Zeng on 9/16/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Component.h"
#include "Sound.h"

class SoundCue{
public:
    SoundCue();
    SoundCue(int channel);
    void Pause();
    void Resume();
    void Stop();
    bool isPlaying();
    bool isPaused();
    
private:
    int mChannel;
};

class AudioComponent : public Component
{
    DECL_COMPONENT(AudioComponent, Component);
    
public:
    
    SoundCue PlaySound(SoundPtr sound, bool looping = false);
    
    AudioComponent(Actor& owner);
};

DECL_PTR(AudioComponent);

