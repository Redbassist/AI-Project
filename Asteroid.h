#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"
#include "PowerUpManager.h"
#include "PowerUp.h"

/*! The Asteroid class.
* These are the obstacles in the game and will float around the level.
* Contain powerups
*/
class Asteroid {
private:
	Texture texture;
	Sprite sprite;
	Pvector pos; /**< Position of the asteroid */
	Pvector direction; /**< The direction that the asteroid is moving */
	float speed; /**< Speed the asteroid is moving across the world */
	float radius;  /**< Size of the asteroid, used for collision detection */
	float health;  /**< Health of the asteroid */
	float rot;  /**< Rotation of the asteroid, spins around at random rate */
public:
	/**
	* Constructor for the Asteroid.
	* Sets the location of the asteroid as well as the starting attributes
	*/
	Asteroid(Pvector);
	
	/**
	* Loads the texture and sprites for the Asteroid.
	* Also sets the radius of the asteroid (random)
	*/
	void LoadAssets();

	/**
	* Calls the movement and wrap around methods of the asteroid.
	* Sets sprite location to the position.
	*/
	void Update();

	/**
	* If the asteroid moves outside the bounds of the level, will be shifted to the otherside of the world.
	*/
	void WrapAround();

	/**
	* The movement of the asteroid is dictated by the random direction that is assigned during construction.
	*/
	void Movement();
	
	/**
	* Draws the asteroid sprite
	*/
	void Draw();
	float getRadius();
	Pvector getPos();
	Pvector getDirection();
	void setDirection(Pvector d);
	float getSpeed();
	/**
	* Checks to see if the life is below 0, if so it will return a bool value of true.
	*/
	bool LifeCheck();
	float getHealth();
	/**
	* Sets health of the asteroid to the sent value
	*/
	void setHealth(float h);
};

#endif