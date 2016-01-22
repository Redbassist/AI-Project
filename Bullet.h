#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Globals.h"

class Bullet {
private:
	Texture texture;
	Sprite sprite; 
	Pvector pos;  /**< Current position of the bullet */
	Pvector direction;  /**< Current direction of the bullet */
	float speed;  /**< max speed of the bullet */
	float radius; /**< Used for the collision detection of the game */
	int ttl; /**< How long the bullet will live for */
	bool destroyed; /**< Shows if the bullet is dead for removal from list  */
public:
	bool playerBullet;
	bool superBullet;
	/**
	* Contructor
	* Creates at given location with sent direction, if it is player bullet and if it is a strong bullet.
	*/
	Bullet(Pvector, Pvector, bool, bool);
	/**
	* Loads sprites and texture of bullet as well as sets radius.
	*/
	void LoadAssets();
	/**
	* calls the variouis update methods of the bullet. Movement / Lifecheck
	*/
	void Update();
	/**
	* Checks if the bullet is still alive (ttl)
	*/
	bool LifeCheck();
	/**
	* Moves the bullet in the direction it was assigned at construction
	*/
	void Movement();
	/**
	* Draws the bullet sprite at current location
	*/
	void Draw();
	Pvector getPos();
	float getRadius();
	void setDestroyed(bool);
};

#endif
