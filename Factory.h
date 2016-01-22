#pragma once  
#include "stdafx.h"
#include "Globals.h"
#include <math.h>
#include "Player.h"
#include "BoidManager.h"
#include "Predator.h"
#include "MissileManager.h"
#include "FactoryManager.h"

class FactoryManger;
/*! The factory  class.
* These are the factories in the game that produce predators.
* Will run from the player and then shoot misisles when nearby to player.
*/

class Factory{
private:
	int id;    /**< id of the current factory. For flocking */
	Player* player;  /**< A reference to the player */
	int width, height; 
	Pvector position;  /**< current pos of the factory */
	Pvector direction;  /**< current direction of the factory */
	float speed;  /**< max speed of the factory */
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Texture texture2; 
	sf::Sprite sprite2;
	Texture uitexture;
	Sprite uisprite;
	float radius;  /**< size of the factory, for circular collision detection */
	float health;   /**< health of the factory */
	int spawnTimer;  /**< Used for timing when new boids will be spawned */
	int shootTimer; /**< how often the factory will shoot missiles */
	int maxMissiles; /**< max missiles allowed to be fired at a time by the factory */
	int missileTimer;  
	Pvector velocity;  /**< current velocity of the factory */
	Pvector acceleration;  /**< current acceleration of the factory */
	float maxSpeed; /**< max speed of the factory */
	float maxForce; /**< max force of the factory */
public:

	State currentState; /**< FSM state of the factory */
	
	/**
	* Constructor for the Factory.
	* Sets the starting attributes for the factory
	*/
	Factory(Player& p, int);

	/**
	* Loads the sprite and texture as well setting the radius of the factory
	*/
	void loadResources();

	/**
	* Draws the factory sprite at current position
	*/
	void Draw();

	/**
	* Draws blip on radar for the factory. 
	* Uses distance from the player scaled down to set on radar.
	*/
	void drawui();

	/**
	* The FSM of the factory. Depending on state ,will call different methods.
	*/
	void Update(vector<Factory*> Factories);

	/**
	* Swaps sides of the world if outside of bounds
	*/
	void WrapAround();

	/**
	* If fleeing from the player and distance is close enough, will start firing missiles.
	*/
	void Shoot(); 

	Pvector normalize(Pvector source); 

	Pvector getPosition();

	float getRadius();

	sf::Sprite getSprite();

	Pvector getDirection();

	float getHealth();

	void setHealth(float h);

	float Distance(Pvector, Pvector); 

	/**
	* Creates predators in the correct flock in groups of 5.
	*/
	void Spawn();

	bool LifeCheck();

	void applyForce(Pvector force);
	/**
	* Checks other factories in the world.
	* If there are any boids with a given range,
	* will get the direction away from those that are close,
	* average them and then apply to acceleration.
	*/
	Pvector Separation(vector<Factory*> Factories);

	/**
	* Will create a vector average from asteroids that are with a range
	* and use this to steer away from asteroids.
	*/
	Pvector AvoidAsteroids();
	/**
	* Averages the current direction of factories within a range.
	* Applies this to current acceleration to makes all point in same direction.
	*/
	Pvector Alignment(vector<Factory*> Factories);
	/**
	* Averages out current locations from boids in a range.
	* Keeps factories in a group.
	*/
	Pvector Cohesion(vector<Factory*> Factories);

	/**
	* Runs from player if close enough
	*/
	Pvector RunFromPlayer(); 
	/**
	* Seeks out given Pvector using current location.
	* Direction from desired - location.
	*/
	Pvector seek(Pvector v);
	/**
	* Method called when the player is near enough and state is RETREAT
	*/
	void run(vector<Factory*> Factories);
	/**
	* Applies the three different methods (seperation / alignment / cohesion)
	* and these are weighted to give certain behavious.
	* These are then applied to the acceleration vector.
	*/
	void flock(vector<Factory*> Factories);
	void flee(vector<Factory*> Factories);
	/**
	* Applies the three different methods (seperation / alignment / cohesion/ avoid methods)
	* and these are weighted to give certain behavious.
	* Called when near asteroids.
	*/
	void avoid(vector<Factory*> Factories);
	float angle(Pvector v);
	/**
	* checks to see if asteroids are nearby
	*/
	bool checkAsteroids();
	/**
	* checks to see if powerups are nearby
	*/
	bool checkPowerUps();
	/**
	* checks to see if player is nearby
	*/
	bool search();
	/**
	* checks to see if player is outside of range
	*/
	bool lost();

};
