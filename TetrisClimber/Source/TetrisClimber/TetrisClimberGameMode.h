// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "TetrisClimberGameMode.generated.h"

UCLASS(minimalapi)
class ATetrisClimberGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATetrisClimberGameMode();
    
    void BeginPlay() override;
	void Tick(float DeltaSeconds)override;
    float GetPositiveXBound() { return positiveXbound; }
    float GetPositiveYBound() { return positiveYbound; }
    float GetPositiveZBound() { return positiveZbound; }
    float GetNegativeXBound() { return negativeXbound; }
    float GetNegativeYBound() { return negativeYbound; }
    float GetNegativeZBound() { return negativeZbound; }
    //AHUD GetHUD() { return HUDClass; }
    //void ToggleHud() { Cast<class AHUD>(HUDClass)->ShowHUD(); }
	void OnTogglePause();
	void OnToggleReset();
    
private:
    float positiveXbound;
    float positiveYbound;
    float positiveZbound;

    float negativeXbound;
    float negativeYbound;
    float negativeZbound;

	bool mPaused = true;
	
};



