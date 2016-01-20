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
#include "BoidManager.h"
#include "Predator.h"
#include "PowerUp.h"
#include "PowerUpManager.h"

class CollisionManager {
private:
	Player* player;
	vector<Asteroid*>* asteroids;
	vector<Bullet*>* bullets;
	vector<Boid*>* boids;
	vector<PowerUp*>* powerups;
public:
	CollisionManager();
	static CollisionManager* GetInstance();
	void CheckCollisions();
	void AsteroidCollisions();
	void BulletCollisions();
	void BoidCollisions();
	void AsteroidPredCollisions();
	void PlayerPowerUpCollisions();
	void setPlayer(Player &p);
	float Distance(Pvector, Pvector);
};

#endif