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
	/**
	* Limit speed when near to the player, to sit near and shoot
	*/
	void LimitAcceleration();
	//Functions involving SFML and visualisation linking 
	void run(vector <Boid*> v);
	void update(vector <Boid*> v);
	void flock(vector <Boid*> v);
	void chase(vector <Boid*> v);
	void collect(vector <Boid*> v);
	void avoid(vector <Boid*> v);
	/**
	* Shoot the player if nearby
	*/
	void Shoot();
	void borders();
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
	//Pvector seek(Pvector v);
};

#endif#pragma once
