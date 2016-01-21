#pragma once
#ifndef BOIDMANANAGER_H
#define BOIDMANANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h" 
#include "Boid.h"
#include "Globals.h"
#include "Swarmer.h"

class BoidManager {
private: 
public:
	BoidManager();
	static BoidManager* GetInstance();
	void AddBoid(Boid*);
	void Update();
	void Draw();
	vector<Boid*> boids;
	vector<Boid*> flocks[4];
};

#endif 
