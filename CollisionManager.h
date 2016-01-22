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
#include "Factory.h"
#include "Missile.h"
#include "PowerUpManager.h"
#include "MissileManager.h"
#include "FactoryManager.h"

/*! The Collision Manager class.
* This manager uses circle based collision detection for collisions between entities.
* Resolves collisions and calls relevant methods of collding entities
*/
class CollisionManager {
private:
	vector<Asteroid*>* asteroids; /**< reference to vector of asteroids */
	vector<Bullet*>* bullets;/**< reference to vector of bullets */
	vector<Boid*>* boids;/**< reference to vector of boids */
	vector<PowerUp*>* powerups;/**< reference to vector of powerups */
	vector<Missile*>* missiles;/**< reference to vector of missiles */
	vector<Factory*>* factories;/**< reference to vector of factories */
public:
	/**
	* Constructor for the CollisionManager.
	* Sets the vector references for all entities
	*/
	CollisionManager();
	/**
	* Returns the instance of the collision manager.
	* Creates an instance if one has not already been created.
	*/
	static CollisionManager* GetInstance();
	/**
	* Calls all the collision methods in the manager
	*/
	void CheckCollisions();
	/**
	* Checks the asteroids collisions (bullets / player)
	*/
	void AsteroidCollisions();
	/**
	* Checks to see what the bullets have collided with
	*/
	void BulletCollisions();
	/**
	* Checks to see what the boids have collided with and calls methods depending on these collisions
	*/
	void BoidCollisions();
	/**
	* Checks the collisions between boids and asteroids
	*/
	void AsteroidPredCollisions();
	/**
	* Checks collision between plaeyr and powerups and powers up player depending on type
	*/
	void PlayerPowerUpCollisions();
	/**
	* Checks colision between player and missile. Damages player
	*/
	void PlayerMissileCollisions();
	/**
	* Checks colision between factory and bullet.
	*/
	void BulletFactoryCollisions();
	void setPlayer(Player &p);
	/**
	* Gets distance between two entites
	*/
	float Distance(Pvector, Pvector);
	Player* player;  /**< A reference to the player */
};

#endif