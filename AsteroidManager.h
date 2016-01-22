#pragma once
#ifndef ASTEROIDMANAGER_H
#define ASTEROIDMANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Player.h"
#include "Asteroid.h"
#include "Globals.h"
#include "BoidManager.h" 

/*! The Asteroid Manager class.
* This manager contains the asteroids of the game.
* Is in charge of creating more asteroids if below amount expected in game.
* Always spawned outside the user's view.
*/
class AsteroidManager {
private:   
	int maxAsteroids; /**< max amount of asteroids in the level */
	float addTimer; /**< How often asteroids are added into the level */
	float timerLength;  /**< Used in tandom with the addTimer */
public:
	/**
	* Constructor for the Asteroid Manager.
	* Sets the starting attributes for the manager
	*/
	AsteroidManager(); 
	/**
	* Returns the instance of the asteroid manager.
	* Creates an instance if one has not already been created.
	*/
	static AsteroidManager* GetInstance();
	/**
	* Updates the asteroid manager, calling the spawn asteroid method.
	*/
	void Update(); 
	/**
	* Draws all the asteroids in the world. 
	*/
	void Draw();
	/**
	* If the amount of asteroids in the world is less than the max, will try and spawn a new one.
	*/
	void SpawnAsteroids();
	/**
	* Used to check if the asteroid is in the view.
	*/
	bool CheckSpawnLocation(Pvector);

	vector<Asteroid*> asteroids;  /**< List of asteroids in the level */
	Player* player;  /**< A reference to the player */
};

#endif