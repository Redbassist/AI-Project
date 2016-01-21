#pragma once
#ifndef PREDATOR_H
#define PREDATOR_H

#include "stdafx.h"  
#include "Boid.h"
#include "BoidManager.h"
#include "Globals.h"
#include "AsteroidManager.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletManager.h"

/*
Predators are part of the flock. This means that when the boids check
through the vector of other boids, the predators are included. Check if
the boid being looked at is a predator, and if it is, calculate a Pvector
that maximizes separation.
*/

class Predator : public Boid
{
private:
	Player* player;
	float fireRate;
	float fireTimer;
public:
	State currentState;
	int window_height;
	int window_width;
	Predator() {}
	Predator(float x, float y, Player*, int);
	~Predator();
	void LoadAssets();
	// Three Laws that boids follow
	Pvector Separation(vector<Boid*> Boids);
	Pvector AvoidAsteroids();
	Pvector Alignment(vector<Boid*> Boids);
	Pvector Cohesion(vector<Boid*> Boids);
	Pvector ChasePlayer();
	Pvector CollectPowerup();
	void LimitAcceleration();
	//Functions involving SFML and visualisation linking 
	void run(vector <Boid*> v);
	void update(vector <Boid*> v);
	void flock(vector <Boid*> v);
	void chase(vector <Boid*> v);
	void collect(vector <Boid*> v);
	void avoid(vector <Boid*> v);
	void Shoot();
	void borders();
	float angle(Pvector v);
	bool checkAsteroids();
	bool checkPowerUps();
	bool search();
	bool lost();
};

#endif#pragma once
