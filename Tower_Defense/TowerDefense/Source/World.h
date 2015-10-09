// World.h
// All actors in the game world should either
// directly or indirectly (by parent) be
// controlled by World

#pragma once
#include <memory>
#include <unordered_set>
#include <vector>
#include "Actor.h"
#include "Enemy.hpp"

class World
{
public:
	World();
	~World();

	void AddActor(ActorPtr actor);

	void Tick(float deltaTime);
	
	void RemoveAllActors();
    
    void AddEnemy(Enemy* enemy){ mEnemySet.emplace(enemy); }
    
    void RemoveEnemy(Enemy* enemy){ mEnemySet.erase(enemy); }
    
    std::vector<Enemy*> GetEnemiesInRange(Vector3 position, float radius){
        std::vector<Enemy*> result;
        for (auto e : mEnemySet) {
            if(e->IsAlive()){
                Vector3 diff = position - e->GetPosition();
                if(diff.LengthSq() < radius*radius){
                    result.push_back(e);
                }
            }
        }
        return result;
    }
    
    Enemy* GetClosestEnemy(Vector3 position, float radius){
        float minDistance = MAXFLOAT;
        Enemy* closestEnemy = nullptr;
        for (auto e : mEnemySet) {
            if(e->IsAlive()){
                Vector3 diff = position - e->GetPosition();
                float distance2 = diff.LengthSq();
                if(distance2 < radius*radius){
                    if(distance2 < minDistance){
                        minDistance = distance2;
                        closestEnemy = e;
                    }
                }
            }
        }
        return closestEnemy;
    }
    
private:
    
	std::unordered_set<ActorPtr> mActors;
    
    std::unordered_set<Enemy*> mEnemySet;
};
