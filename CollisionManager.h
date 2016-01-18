#pragma once
#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Asteroid.h"
#include "Globals.h"
#include "AsteroidManager.h"

class CollisionManager {
private:
	Player* player;
	vector<Asteroid*>* asteroids;
public:
	CollisionManager();
	static CollisionManager* GetInstance();
	void CheckCollisions();
	void setPlayer(Player &p);
};

#endif