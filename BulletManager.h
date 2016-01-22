#pragma once
#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Globals.h"
/*! The Bullet Manager class.
* This manager contains the Bullets of the game.
* In charge of updating and drawing bullets in the game.
*/
class BulletManager {
private:  
public:
	/**
	* Constructor for the Bullet Manager.
	* Sets the starting attributes for the manager
	*/
	BulletManager();
	/**
	* Returns the instance of the bullet manager.
	* Creates an instance if one has not already been created.
	*/
	static BulletManager* GetInstance();
	/**
	* Adds a bullet to the bullet list
	*/
	void AddBullet(Bullet*);
	/**
	* Updates the bullets as well as removing them from the list if they are to be destroyed.
	*/
	void Update();
	void Draw();
	vector<Bullet*> bullets; /**< List of bullets in the world */
};

#endif