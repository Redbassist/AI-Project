#pragma once 
#ifndef SWARMER_H
#define SWARMER_H

#include "stdafx.h"  
#include "Boid.h"
#include "Globals.h"
#include "AsteroidManager.h"
#include "Player.h" 

/*
Predators are part of the flock. This means that when the boids check
through the vector of other boids, the predators are included. Check if
the boid being looked at is a predator, and if it is, calculate a Pvector
that maximizes separation.
*/

class Swarmer : public Boid
{
private:
	Player* player; 
public:
	State currentState;
	int window_height;
	int window_width;
	Swarmer() {}
	Swarmer(float x, float y, Player*);
	~Swarmer();
	void LoadAssets(); 
	Pvector Separation(vector<Boid*> Boids);
	Pvector AvoidAsteroids();
	Pvector Alignment(vector<Boid*> Boids);
	Pvector Cohesion(vector<Boid*> Boids);
	Pvector ChasePlayer(); 
	//Functions involving SFML and visualisation linking 
	void run(vector <Boid*> v);
	void update(vector <Boid*> v); 
	void avoid(vector <Boid*> v); 
	void chase(vector <Boid*> v);
	void borders();
	float angle(Pvector v);
	bool checkAsteroids();
	bool search();
	bool lost();
	void swarm(vector <Boid*> v);
};

#endif#pragma once
