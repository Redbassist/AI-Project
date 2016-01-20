
#ifndef BOID_H_
#define BOID_H_

#include "stdafx.h"  
#include "Globals.h"
#include "Player.h"
 
/*
Brief description of Boid Class:
//The boid class creates an object which has multiple vectors (location, velocity, acceleration)
//Boids are given certain properties and rules to follow when interacting with other boids.
//They are being used to simulate flocking patterns and properties

For predators:
Predators should act by themselves. Thus, their maxSpeed should be different,
and they should not adhere to the three laws. However, they should adhere to
cohesion -- how should they seek prey? Also, prey should be able to run away
from predators. There are several ways to do this. The easiest way:

Predators are part of the flock. This means that when the boids check
through the vector of other boids, the predators are included. Check if
the boid being looked at is a predator, and if it is, calculate a Pvector
that maximizes separation.
*/

class Boid
{
private:
public: 
	Texture texture;
	Sprite sprite;
	Pvector location;
	float radius;
	Pvector velocity;
	Pvector acceleration;
	float maxSpeed;
	float maxForce;
	float health;
	Boid() {}
	Boid(float x, float y)
	{
	}
	Boid(float x, float y, Player*)
	{
	}
	Boid::~Boid()
	{
	//cout << "Boid is being deleted by destructor!" << endl;
	}

	void applyForce(Pvector force);
	// Three Laws that boids follow
	virtual Pvector Separation(vector<Boid*> Boids);
	virtual Pvector Alignment(vector<Boid*> Boids);
	virtual Pvector Cohesion(vector<Boid*> Boids);
	//Functions involving SFML and visualisation linking
	Pvector seek(Pvector v);
	virtual void run(vector <Boid*> v);
	virtual void update(vector <Boid*> v);
	void draw();
	virtual void flock(vector <Boid*> v);
	void borders();
	float angle(Pvector v);
	void swarm(vector <Boid*> v);
	float getHealth();
	void setHealth(float h);
	float getRadius();
	Pvector getPos();
	bool LifeCheck();
};

#endif#pragma once
