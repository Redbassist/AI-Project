#pragma once
#ifndef PREDATOR_H
#define PREDATOR_H

#include "stdafx.h"  
#include "Boid.h"
#include "Globals.h"
#include "Player.h"
/*
Predators are part of the flock. This means that when the boids check
through the vector of other boids, the predators are included. Check if
the boid being looked at is a predator, and if it is, calculate a Pvector
that maximizes separation.
*/

enum State {
	SEARCH,
	ATTACK,
	RETREAT
};
 
class Predator : public Boid
{
private:
	Player* player;
public:
	State currentState;
	int window_height;
	int window_width;
	Predator() {}
	Predator(float x, float y, Player*);
	~Predator();
	void LoadAssets();
	// Three Laws that boids follow
	Pvector Separation(vector<Boid*> Boids);
	Pvector Alignment(vector<Boid*> Boids);
	Pvector Cohesion(vector<Boid*> Boids);
	//Functions involving SFML and visualisation linking 
	void run(vector <Boid*> v);
	void update(vector <Boid*> v);
	void flock(vector <Boid*> v);
	void borders();
	float angle(Pvector v);
	bool search();
	bool lost();
	//void swarm(vector <Boid*> v);
};

#endif#pragma once
