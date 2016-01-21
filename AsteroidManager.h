#pragma once
#ifndef ASTEROIDMANAGER_H
#define ASTEROIDMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Asteroid.h"
#include "Globals.h"
#include "BoidManager.h"
#include "CollisionManager.h"

class AsteroidManager {
private:   
	int maxAsteroids;
	float addTimer;
	float timerLength;
public:
	AsteroidManager(); 
	static AsteroidManager* GetInstance();
	void Update(); 
	void Draw();
	void SpawnAsteroids();
	bool CheckSpawnLocation(Pvector);
	vector<Asteroid*> asteroids;
};

#endif