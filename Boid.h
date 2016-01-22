
#ifndef BOID_H_
#define BOID_H_

#include "stdafx.h"  
#include "Globals.h"
#include "Player.h"
 
/*! The Boid class.
* These are the boids in the game and predators and swarmers inherit from them.
* Contains basic AI that these build on.
*/

class Boid
{
private:
public:
	Player* player; /**< Reference to the player */
	int powerTimer; /**< Dictates how long that the bullets produced are produced as super bullets. */ 
	int speedTimer;  /**< Increased speed lasts as long as this is over 0 */
	int currentFlock;  /**< the id of the current flock. Assigned depending on the factory that it is produced from */
	Texture texture; 
	Sprite sprite;
	Texture uitexture;
	Sprite uisprite;
	Pvector location;  /**< Position vector */
	float radius;  /**< Size and used for the collisions */
	Pvector velocity;  /**< Current velocity. Used for moving. */
	Pvector acceleration;  /**< The vector that is affected by the different AI methods. Then added to velocity */
	float maxSpeed;  /**< Used to limit speed.  */
	float maxForce;  /**<  Used to limit force */
	float health;

	Boid() {}

	Boid(float x, float y)
	{
	}
	/**
	* Contrustor for the boid.
	* Sets the start position as well as reference to the player.
	* Sent the current flock number as well when created by factory.
	*/
	Boid(float x, float y, Player*, int)
	{
	}
	/**
	* Destructor of the boid.
	*/
	Boid::~Boid()
	{
	//cout << "Boid is being deleted by destructor!" << endl;
	}

	/**
	* Adds sent vector onto the acceleration vector
	*/
	void applyForce(Pvector force);

	/**
	* Checks other boids in the world.
	* If there are any boids with a given range,
	* will get the direction away from those that are close,
	* average them and then apply to acceleration.
	*/
	virtual Pvector Separation(vector<Boid*> Boids);
	/** 
	* Averages the current direction of boids within a range.
	* Applies this to current acceleration to makes all point in same direction.
	*/
	virtual Pvector Alignment(vector<Boid*> Boids);
	/**
	* Averages out current locations from boids in a range.
	* Keeps boids in a group.
	*/
	virtual Pvector Cohesion(vector<Boid*> Boids);

	/**
	* Seeks out given Pvector using current location.
	* Direction from desired - location.
	*/
	Pvector seek(Pvector v);
	/**
	* General flocking method that calls flock as well as 
	* the border check
	*/
	virtual void run(vector <Boid*> v);
	/**
	* Holds the finite state machine of the boid.
	* Moves through different behavious depending on the situation.
	* SEARCH / ATTACK / AVOID 
	*/
	virtual void update(vector <Boid*> v);
	/**
	* Draws the boid sprite using current position
	* and velocity for rotation
	*/
	void draw();
	/**
	* Draws dot on radar in relation to centre of the radar.
	* Will not be drawn if far enough away.
	*/
	void drawui();
	/**
	* Applies the three different methods (seperation / alignment / cohesion)
	* and these are weighted to give certain behavious.
	* These are then applied to the acceleration vector.
	*/
	virtual void flock(vector <Boid*> v);
	/**
	* Wrap around check, teleports to otherside of world if out of bounds.
	*/
	void borders();
	/**
	* Gets the radian angle of sent pVector
	*/
	float angle(Pvector v);
	/**
	* Implements swarming equation.
	*/
	void swarm(vector <Boid*> v); 
	float getHealth();
	void setHealth(float h);
	float getRadius();
	Pvector getPos();
	/**
	* Checks the current life of the boid, returns true if under 0.
	*/
	bool LifeCheck();
};

#endif#pragma once
