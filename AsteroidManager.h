#pragma once
#ifndef ASTEROIDMANAGER_H
#define ASTEROIDMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Asteroid.h"
#include "Globals.h"

class AsteroidManager {
private:  
	Player* player;
	int maxAsteroids;
	float addTimer;
	float timerLength;
	vector<Asteroid*> asteroids;
public:
	AsteroidManager(); 
	static AsteroidManager* GetInstance();
	void Update(); 
	void Draw();
	void SpawnAsteroids();
};

#endif