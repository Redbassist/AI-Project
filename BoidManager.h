#pragma once
#ifndef BOIDMANANAGER_H
#define BOIDMANANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h" 
#include "Boid.h"
#include "Globals.h"
#include "Swarmer.h"
/*! The Boid  Manager class.
* This manager contains the boids of the game.
* Is in charge of updating and drawing.
* Contains the different flocks as well.
*/
class BoidManager {
private:
public:
	/**
	* Constructor for the BoidManager.
	* Sets the starting attributes for the manager
	*/
	BoidManager();
	/**
	* Returns the instance of the boid manager.
	* Creates an instance if one has not already been created.
	*/
	static BoidManager* GetInstance();
	/**
	* Add boid to the boids list and to the boids flocks as well.
	*/
	void AddBoid(Boid*);
	/**
	* Updating the boids in the list.
	*/
	void Update();
	/**
	* Drawing the boids in the list
	*/
	void Draw();
	/**
	* Calling the drawui method of all the boids
	*/
	void DrawUI(); 
	vector<Boid*> boids; /**< Contains all the worlds boids */
	vector<Boid*> flocks[4]; /**< Seperate vector for each flock of boids in the world. Corresponds to each factory */
};

#endif 
