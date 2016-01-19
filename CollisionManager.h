#pragma once
#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Asteroid.h"
#include "Globals.h"
#include "AsteroidManager.h"
#include "Bullet.h"
#include "BulletManager.h"

class CollisionManager {
private:
	Player* player;
	vector<Asteroid*>* asteroids;
	vector<Bullet*>* bullets;
public:
	CollisionManager();
	static CollisionManager* GetInstance();
	void CheckCollisions();
	void AsteroidCollisions();
	void BulletCollisions();
	void setPlayer(Player &p);
	float Distance(Pvector, Pvector);
};

#endif