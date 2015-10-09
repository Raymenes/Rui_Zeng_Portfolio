// Game.h
// Contains the core game functionality
// such as the game loop

#pragma once
#include "Renderer.h"
#include "FrameTimer.h"
#include "World.h"
#include "AssetCache.h"
#include "PhysWorld.h"
#include "InputManager.h"
#include "GameTimers.h"
#include "GameMode.hpp"

class Game
{
public:
	Game();
	~Game();
	bool Init();
	void RunLoop();
	void Quit();

	Renderer& GetRenderer() { return mRenderer; }
	World& GetWorld() { return mWorld; }
	AssetCache& GetAssetCache() { return mAssetCache; }
	PhysWorld& GetPhysWorld() { return mPhysWorld; }
    InputManager& GetInput() { return mInput; }
    GameTimerManager& GetGameTimerManager() { return mGameTimerManager; }
    GameModePtr& GetGameMode(){ return mGameMode; }
    
    
private:
	void StartGame();
	
	void ProcessInput();
	void HandleKeyPressed(int key);
	void HandleKeyReleased(int key);

	void Tick();
	void GenerateOutput();
    
    void AddInputMappings();

	Renderer mRenderer;
	FrameTimer mTimer;
	World mWorld;
	AssetCache mAssetCache;
	PhysWorld mPhysWorld;
//    ShipPtr mShip;
    InputManager mInput;
    GameTimerManager mGameTimerManager;
    GameModePtr mGameMode;

	bool mShouldQuit;
};
